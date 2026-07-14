// Copyright Heartape


#include "Gameplay/Ability/Player/ComboGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Animation/AnimInstance.h"
#include "Gameplay/Effect/ComboCooldownGameplayEffect.h"
#include "Gameplay/Ability.h"


UComboGameplayAbility::UComboGameplayAbility()
{
	TArray<FGameplayTag> AbilityTagArray = {Ability::Combo};
	SetAssetTags(FGameplayTagContainer::CreateFromArray(AbilityTagArray));

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// 由 GameplayEvent(Ability.Combo) 激活：同一事件在能力激活期间也会广播到监听任务，用于连击输入缓存
	FAbilityTriggerData& Trigger = AbilityTriggers.Add_GetRef(FAbilityTriggerData());
	Trigger.TriggerTag = Ability::Combo;

	CooldownGameplayEffectClass = UComboCooldownGameplayEffect::StaticClass();
	CooldownDuration = 0.5f;
	CostAmount = 5.0f;
}

void UComboGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!CommitAbilityChecked(Handle, ActorInfo, ActivationInfo))
	{
		return;
	}

	// 初始化连击状态
	ComboCount = 0;
	bHasBufferedInput = false;
	bIgnoreFirstInput = true;

	// 监听连击中的再次按键事件（能力激活期间，HandleGameplayEvent 会广播到该任务）
	if (UAbilityTask_WaitGameplayEvent* InputTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Ability::Combo, nullptr, false, true))
	{
		InputTask->EventReceived.AddDynamic(this, &UComboGameplayAbility::OnComboInput);
		InputTask->ReadyForActivation();
	}

	// 监听段落推进事件（由 AnimNotify_CheckCombo 发送）
	if (UAbilityTask_WaitGameplayEvent* CheckTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Ability::ComboCheck, nullptr, false, true))
	{
		CheckTask->EventReceived.AddDynamic(this, &UComboGameplayAbility::OnComboCheck);
		CheckTask->ReadyForActivation();
	}

	UAnimInstance* AnimInstance = ActorInfo && ActorInfo->GetAnimInstance() ? ActorInfo->GetAnimInstance() : nullptr;
	if (!AttackMontage || !AnimInstance)
	{
		// 没有蒙太奇也允许纯逻辑触发：立即执行一次检测后结束
		StartTrace();
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	AnimInstance->Montage_Play(AttackMontage, 1.0f);
	if (ComboSectionNames.IsValidIndex(0))
	{
		AnimInstance->Montage_JumpToSection(ComboSectionNames[0], AttackMontage);
	}

	AnimInstance->OnMontageEnded.AddDynamic(this, &UComboGameplayAbility::OnMontageEnded);

	StartTrace();
}

void UComboGameplayAbility::OnComboInput(FGameplayEventData Payload)
{
	// 激活当帧的首次事件广播会被忽略，避免激活输入被误判为连击推进
	if (bIgnoreFirstInput)
	{
		bIgnoreFirstInput = false;
		return;
	}
	bHasBufferedInput = true;
}

void UComboGameplayAbility::OnComboCheck(FGameplayEventData Payload)
{
	if (!bHasBufferedInput)
	{
		return;
	}
	bHasBufferedInput = false;

	++ComboCount;

	if (ComboCount < ComboSectionNames.Num())
	{
		if (const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo())
		{
			if (UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance())
			{
				AnimInstance->Montage_JumpToSection(ComboSectionNames[ComboCount], AttackMontage);
			}
		}

		// 新段落重新开启命中检测
		StartTrace();
	}
}

void UComboGameplayAbility::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 仅响应本能力播放的蒙太奇结束，避免其他蒙太奇（如跳跃）误触结束能力
	if (Montage != AttackMontage)
	{
		return;
	}

	if (const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo())
	{
		if (UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance())
		{
			AnimInstance->OnMontageEnded.RemoveDynamic(this, &UComboGameplayAbility::OnMontageEnded);
		}
	}

	StopTrace();

	EndAbility(CurrentSpecHandle, GetCurrentActorInfo(), CurrentActivationInfo, true, false);
}

void UComboGameplayAbility::StartTrace()
{
	StopTrace();
	AlreadyHitActors.Reset();
	GetWorld()->GetTimerManager().SetTimer(TraceTimerHandle, this, &UComboGameplayAbility::PerformTrace, TraceInterval, true);
	if (HitWindow > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(StopTraceHandle, this, &UComboGameplayAbility::StopTrace, HitWindow, false);
	}
}

void UComboGameplayAbility::StopTrace()
{
	GetWorld()->GetTimerManager().ClearTimer(TraceTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(StopTraceHandle);
}

void UComboGameplayAbility::PerformTrace()
{
	const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
	if (!ActorInfo || !DamageEffectClass)
	{
		return;
	}

	TArray<AActor*> HitActors = SphereTrace(ActorInfo, TraceRadius, TraceRange);
	for (AActor* HitActor : HitActors)
	{
		if (!IsValid(HitActor) || AlreadyHitActors.Contains(HitActor))
		{
			continue;
		}
		AlreadyHitActors.Add(TWeakObjectPtr<AActor>(HitActor));
		ApplyEffectToActor(ActorInfo, DamageEffectClass, HitActor, -DamageMultiplier);
	}
}
