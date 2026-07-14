// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "BaseGameplayAbility.h"
#include "DashGameplayAbility.generated.h"

class UAnimMontage;
struct FGameplayEventData;

/**
 * 冲刺能力：由 Ability.Dash 事件激活。
 * - 激活时施加 State.Dashing 标签、关闭重力并清零速度，播放冲刺蒙太奇
 * - 蒙太奇中的 AnimNotify_EndDash 发送 DashEnd 事件，据此恢复重力、移除标签并结束能力
 */
UCLASS()
class RPG_API UDashGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UDashGameplayAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION()
	void OnDashEnd(FGameplayEventData Payload);

	// 恢复重力并移除 State.Dashing 标签
	void RestoreDashState();

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Ability|Dash")
	TObjectPtr<UAnimMontage> DashMontage;

	// 冲刺期间保存的重力缩放，结束后恢复
	float SavedGravityScale = 1.0f;
};
