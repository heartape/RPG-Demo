// Copyright Heartape


#include "Gameplay/Ability/Player/ChargedGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Animation/AnimInstance.h"
#include "Gameplay/Effect/ChargedCooldownGameplayEffect.h"
#include "Gameplay/Ability.h"


UChargedGameplayAbility::UChargedGameplayAbility()
{
	TArray<FGameplayTag> AbilityTagArray = {Ability::Charged};
	SetAssetTags(FGameplayTagContainer::CreateFromArray(AbilityTagArray));

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// 由 GameplayEvent(Ability.Charged) 激活（按下）
	FAbilityTriggerData& Trigger = AbilityTriggers.Add_GetRef(FAbilityTriggerData());
	Trigger.TriggerTag = Ability::Charged;

	CooldownGameplayEffectClass = UChargedCooldownGameplayEffect::StaticClass();
	CooldownDuration = 2.0f;
	CostAmount = 15.0f;
}

void UChargedGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbilityChecked(Handle, ActorInfo, ActivationInfo))
	{
		return;
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	bIsCharging = true;
	bReleased = false;

	UAnimInstance* AnimInstance = ActorInfo && ActorInfo->GetAnimInstance() ? ActorInfo->GetAnimInstance() : nullptr;
	if (!AttackMontage || !AnimInstance)
	{
		PerformHeavyTrace();
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	AnimInstance->Montage_Play(AttackMontage, 1.0f);

	// 进入蓄力循环，监听循环检查与松开事件
	AnimInstance->Montage_JumpToSection(ChargeLoopSection, AttackMontage);

	if (UAbilityTask_WaitGameplayEvent* CheckTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Ability::ChargedCheck, nullptr, false, true))
	{
		CheckTask->EventReceived.AddDynamic(this, &UChargedGameplayAbility::OnChargedCheck);
		CheckTask->ReadyForActivation();
	}

	if (UAbilityTask_WaitGameplayEvent* ReleaseTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Ability::ChargedRelease, nullptr, false, true))
	{
		ReleaseTask->EventReceived.AddDynamic(this, &UChargedGameplayAbility::OnChargedRelease);
		ReleaseTask->ReadyForActivation();
	}

	AnimInstance->OnMontageEnded.AddDynamic(this, &UChargedGameplayAbility::OnMontageEnded);
}

void UChargedGameplayAbility::OnChargedCheck(FGameplayEventData Payload)
{
	// 已释放，忽略迟到的循环通知
	if (bReleased) return;

	if (bIsCharging)
	{
		// 仍按住：继续蓄力循环
		if (const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo())
		{
			if (UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance())
			{
				AnimInstance->Montage_JumpToSection(ChargeLoopSection, AttackMontage);
			}
		}
	}
	else
	{
		// 已松开：释放
		ReleaseAttack();
	}
}

void UChargedGameplayAbility::OnChargedRelease(FGameplayEventData Payload)
{
	// 清除按住标记
	bIsCharging = false;

	if (bReleased) return;
	ReleaseAttack();
}

void UChargedGameplayAbility::ReleaseAttack()
{
	if (bReleased) return;
	bReleased = true;
	// 跳转到释放攻击段落
	if (const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo())
	{
		if (UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance())
		{
			if (ChargeAttackSection != NAME_None)
			{
				AnimInstance->Montage_JumpToSection(ChargeAttackSection, AttackMontage);
			}
		}
	}

	// 启动命中检测
	AlreadyHitActors.Reset();
	GetWorld()->GetTimerManager().ClearTimer(TraceHandle);
	if (WindUp > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(TraceHandle, this, &UChargedGameplayAbility::PerformHeavyTrace, WindUp, false);
	}
	else
	{
		PerformHeavyTrace();
	}
}

void UChargedGameplayAbility::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage != AttackMontage)
	{
		return;
	}

	if (const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo())
	{
		if (UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance())
		{
			AnimInstance->OnMontageEnded.RemoveDynamic(this, &UChargedGameplayAbility::OnMontageEnded);
		}
	}

	GetWorld()->GetTimerManager().ClearTimer(TraceHandle);
	AlreadyHitActors.Reset();

	EndAbility(CurrentSpecHandle, GetCurrentActorInfo(), CurrentActivationInfo, true, false);
}

void UChargedGameplayAbility::PerformHeavyTrace()
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
