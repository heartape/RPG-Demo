// Copyright Heartape


#include "Gameplay/Ability/Player/BaseGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Gameplay/PlayerAttributeSet.h"
#include "Gameplay/Effect/CostGameplayEffect.h"
#include "Gameplay/Ability.h"
#include "GameplayEffectTypes.h"
#include "Kismet/KismetSystemLibrary.h"


UBaseGameplayAbility::UBaseGameplayAbility()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;

	// 默认绑定到项目提供的 C++ 消耗 GE 类；冷却 GE 由各子类指定具体子类
	CostGameplayEffectClass = UCostGameplayEffect::StaticClass();
}

bool UBaseGameplayAbility::CommitAbilityChecked(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return false;
	}
	return true;
}

TArray<AActor*> UBaseGameplayAbility::SphereTrace(const FGameplayAbilityActorInfo* ActorInfo,
    float Radius, float Range) const
{
	TArray<AActor*> HitActors;
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		return HitActors;
	}

	AActor* Avatar = ActorInfo->AvatarActor.Get();
	if (!IsValid(Avatar))
	{
		return HitActors;
	}

	const FVector Start = Avatar->GetActorLocation();
	const FVector Forward = Avatar->GetActorForwardVector();
	const FVector End = Start + Forward * Range;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Avatar);

	TArray<FHitResult> HitResults;
	const ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	const bool bHit = UKismetSystemLibrary::SphereTraceMulti(
		Avatar,
		Start,
		End,
		Radius,
		TraceChannel,
		false,
		ActorsToIgnore,
		bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		HitResults,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		2.0f);

	if (!bHit)
	{
		return HitActors;
	}

	TArray<AActor*> UniqueActors;
	for (const FHitResult& Hit : HitResults)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if (!UniqueActors.Contains(HitActor))
			{
				UniqueActors.Add(HitActor);
			}
		}
	}

	return UniqueActors;
}

void UBaseGameplayAbility::ApplyEffectToActor(const FGameplayAbilityActorInfo* ActorInfo,
    TSubclassOf<UGameplayEffect> EffectClass, AActor* TargetActor, float Multiplier) const
{
	if (!EffectClass || !IsValid(TargetActor) || !ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
	{
		return;
	}

	UAbilitySystemComponent* SourceASC = ActorInfo->AbilitySystemComponent.Get();
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!SourceASC || !IsValid(TargetASC))
	{
		return;
	}

	FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
	ContextHandle.AddInstigator(SourceASC->GetOwnerActor(), ActorInfo->AvatarActor.Get());

	const float EffectLevel = 1.0f;
	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(EffectClass, EffectLevel, ContextHandle);
	if (!SpecHandle.IsValid())
	{
		return;
	}

	SpecHandle.Data.Get()->SetSetByCallerMagnitude(Ability::Data::Magnitude, Multiplier);
	SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
}

void UBaseGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	if (!CooldownGameplayEffectClass || !ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
	{
		return;
	}

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	const FGameplayEffectContextHandle Context = MakeEffectContext(Handle, ActorInfo);
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CooldownGameplayEffectClass, GetAbilityLevel(Handle, ActorInfo), Context);
	if (!SpecHandle.IsValid())
	{
		return;
	}

	// 注入冷却时长（SetByCaller）；冷却标签已由各 CooldownGE 子类在 CDO 上烘焙
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(Ability::Data::Magnitude, CooldownDuration);

	ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
}

bool UBaseGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	// 无消耗直接放行
	if (CostAmount <= 0.0f)
	{
		return true;
	}

	if (!ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
	{
		return false;
	}

	// SetByCaller 在 CDO 上无值，引擎默认的 CanApplyAttributeModifiers 无法正确估算，
	// 因此直接读取 Mana 属性判断是否足够。
	const UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (const UPlayerAttributeSet* AttrSet = Cast<UPlayerAttributeSet>(ASC->GetAttributeSet(UPlayerAttributeSet::StaticClass())))
	{
		if (AttrSet->GetMana() < CostAmount)
		{
			if (OptionalRelevantTags)
			{
				OptionalRelevantTags->AddTag(UAbilitySystemGlobals::Get().ActivateFailCostTag);
			}
			return false;
		}
		return true;
	}

	return false;
}

void UBaseGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	if (CostAmount <= 0.0f || !ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
	{
		return;
	}

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	const FGameplayEffectContextHandle Context = MakeEffectContext(Handle, ActorInfo);
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CostGameplayEffectClass, GetAbilityLevel(Handle, ActorInfo), Context);
	if (!SpecHandle.IsValid())
	{
		return;
	}

	// 以负数施加，扣减 Mana
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(Ability::Data::Magnitude, -CostAmount);

	ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
}