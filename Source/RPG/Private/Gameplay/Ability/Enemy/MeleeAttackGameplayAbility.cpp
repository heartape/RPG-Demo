// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Ability/Enemy/MeleeAttackGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AI/RPGGameplayTags.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "Kismet/KismetSystemLibrary.h"

UMeleeAttackGameplayAbility::UMeleeAttackGameplayAbility()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;

	FGameplayTagContainer AssetTags;
	AssetTags.AddTag(FRPGGameplayTags::Get().Ability_Attack_Melee);
	SetAssetTags(AssetTags);

	FGameplayTagContainer OwnedTags;
	OwnedTags.AddTag(FRPGGameplayTags::Get().State_Attacking);
	ActivationOwnedTags = OwnedTags;
}

void UMeleeAttackGameplayAbility::PostLoad()
{
	Super::PostLoad();

	FGameplayTagContainer& TagsRef = EditorGetAssetTags();
	if (!TagsRef.HasTag(FRPGGameplayTags::Get().Ability_Attack_Melee))
	{
		TagsRef.AddTag(FRPGGameplayTags::Get().Ability_Attack_Melee);
		UE_LOG(LogTemp, Log, TEXT("GA_MeleeAttack::PostLoad: forced added Ability.Attack.Melee"));
	}
}

bool UMeleeAttackGameplayAbility::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (!bResult)
	{
		UE_LOG(LogTemp, Error, TEXT("GA_MeleeAttack::CanActivateAbility FAILED. Owner=%s Avatar=%s ASC=%s"),
			*GetNameSafe(ActorInfo ? ActorInfo->OwnerActor.Get() : nullptr),
			*GetNameSafe(ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr),
			*GetNameSafe(ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr));
		if (OptionalRelevantTags)
		{
			UE_LOG(LogTemp, Error, TEXT("  FailureTags: %s"), *OptionalRelevantTags->ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("GA_MeleeAttack::CanActivateAbility OK"));
	}
	return bResult;
}

void UMeleeAttackGameplayAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Log, TEXT("GA_MeleeAttack::ActivateAbility called"));

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Error, TEXT("GA_MeleeAttack: CommitAbility failed"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (AttackMontage)
	{
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this, NAME_None, AttackMontage);
		if (MontageTask)
		{
			MontageTask->OnCompleted.AddDynamic(this, &UMeleeAttackGameplayAbility::OnMontageCompleted);
			MontageTask->OnInterrupted.AddDynamic(this, &UMeleeAttackGameplayAbility::OnMontageInterrupted);
			MontageTask->ReadyForActivation();
		}
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	FGameplayTag EventTag = FRPGGameplayTags::Get().Event_AttackHit;
	UAbilityTask_WaitGameplayEvent* WaitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, EventTag, nullptr, true, true);
	if (WaitTask)
	{
		WaitTask->EventReceived.AddDynamic(this, &UMeleeAttackGameplayAbility::HandleAttackHit);
		WaitTask->ReadyForActivation();
	}
}

void UMeleeAttackGameplayAbility::OnMontageCompleted()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UMeleeAttackGameplayAbility::OnMontageInterrupted()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
}

void UMeleeAttackGameplayAbility::HandleAttackHit(const FGameplayEventData Payload)
{
	ApplyDamageToTargetsInRadius();
}

void UMeleeAttackGameplayAbility::ApplyDamageToTargetsInRadius()
{
	if (!DamageEffectClass)
	{
		return;
	}

	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor)
	{
		return;
	}

	UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(AvatarActor);
	if (!SourceASC)
	{
		return;
	}

	FVector Origin = AvatarActor->GetActorLocation();
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(AvatarActor);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<AActor*> HitActors;
	UKismetSystemLibrary::SphereOverlapActors(
		this, Origin, HitDetectRadius, ObjectTypes, APawn::StaticClass(), IgnoredActors, HitActors);

	for (AActor* HitActor : HitActors)
	{
		if (HitActor == AvatarActor)
		{
			continue;
		}

		UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(HitActor);
		if (!TargetASC)
		{
			continue;
		}

		FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
			DamageEffectClass, 1.f, SourceASC->MakeEffectContext());
		if (SpecHandle.IsValid())
		{
			SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
		}
	}
}
