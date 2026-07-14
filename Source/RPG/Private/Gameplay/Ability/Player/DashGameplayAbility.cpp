// Copyright Heartape


#include "Gameplay/Ability/Player/DashGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/Effect/DashCooldownGameplayEffect.h"
#include "Gameplay/Ability.h"


UDashGameplayAbility::UDashGameplayAbility()
{
	TArray<FGameplayTag> AbilityTagArray = {Ability::Dash};
	SetAssetTags(FGameplayTagContainer::CreateFromArray(AbilityTagArray));

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// 由 GameplayEvent(Ability.Dash) 激活
	FAbilityTriggerData& Trigger = AbilityTriggers.Add_GetRef(FAbilityTriggerData());
	Trigger.TriggerTag = Ability::Dash;

	CooldownGameplayEffectClass = UDashCooldownGameplayEffect::StaticClass();
	CooldownDuration = 1.0f;
	CostAmount = 8.0f;
}

void UDashGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbilityChecked(Handle, ActorInfo, ActivationInfo))
	{
		return;
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	ACharacter* AvatarCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo());

	if (!IsValid(ASC) || !IsValid(AvatarCharacter))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// 施加冲刺中状态标签
	ASC->AddLooseGameplayTag(Ability::State::Dashing);

	// 关闭重力并清零速度
	UCharacterMovementComponent* Movement = AvatarCharacter->GetCharacterMovement();
	SavedGravityScale = Movement->GravityScale;
	Movement->GravityScale = 0.0f;
	Movement->Velocity = FVector::ZeroVector;

	// 监听 DashEnd 事件（由蒙太奇中的 AnimNotify_EndDash 发送）以恢复重力并结束能力
	if (UAbilityTask_WaitGameplayEvent* DashEndTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Ability::DashEnd, nullptr, false, true))
	{
		DashEndTask->EventReceived.AddDynamic(this, &UDashGameplayAbility::OnDashEnd);
		DashEndTask->ReadyForActivation();
	}

	// 播放冲刺蒙太奇
	if (UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance())
	{
		AnimInstance->Montage_Play(DashMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
	}
}

void UDashGameplayAbility::OnDashEnd(FGameplayEventData Payload)
{
	// AnimNotify_EndDash 触发：恢复重力与状态并结束能力
	if (const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo())
	{
		if (UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance())
		{
			AnimInstance->Montage_Stop(0.0f, DashMontage);
		}
	}

	RestoreDashState();
	EndAbility(CurrentSpecHandle, GetCurrentActorInfo(), CurrentActivationInfo, true, false);
}

void UDashGameplayAbility::RestoreDashState()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		ASC->RemoveLooseGameplayTag(Ability::State::Dashing);
	}

	if (ACharacter* AvatarCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		if (UCharacterMovementComponent* Movement = AvatarCharacter->GetCharacterMovement())
		{
			Movement->GravityScale = SavedGravityScale;
		}
	}
}
