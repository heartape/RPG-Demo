// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "BaseGameplayAbility.h"
#include "ChargedGameplayAbility.generated.h"

class UAnimMontage;
class UGameplayEffect;
struct FGameplayEventData;

/**
 * 蓄力攻击能力：由 Ability.Charged 事件激活（按下），由 Ability.ChargedRelease 事件释放（松开）。
 * - 按下进入蓄力循环段落，松开（或循环中的检查）跳转到释放攻击段落
 * - 蓄力循环中由 AnimNotify_CheckChargedAttack 发送 Ability.ChargedCheck 事件：
 *   仍按住则继续循环，已松开则释放
 * - 释放时在 WindUp 延迟后做一次性大范围检测并施加伤害 GE
 */
UCLASS()
class RPG_API UChargedGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UChargedGameplayAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Ability")
	TObjectPtr<UAnimMontage> AttackMontage;

	// 蓄力循环段落名
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Ability|Charged")
	FName ChargeLoopSection = "Charge";

	// 蓄力释放攻击段落名
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Ability|Charged")
	FName ChargeAttackSection = "Attack";

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Ability")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay|Ability", meta = (ClampMin = "0.0"))
	float DamageMultiplier = 2.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay|Ability", meta = (ClampMin = "0.0"))
	float TraceRadius = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay|Ability", meta = (ClampMin = "0.0"))
	float TraceRange = 250.0f;

	// 释放后到命中检测的延迟（秒），用于对齐挥砍帧
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Ability", meta = (ClampMin = "0.0"))
	float WindUp = 0.1f;

private:
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// 蓄力循环检查事件回调（由 AnimNotify_CheckChargedAttack 发送）
	UFUNCTION()
	void OnChargedCheck(FGameplayEventData Payload);

	// 松开蓄力键事件回调（由控制器发送）
	UFUNCTION()
	void OnChargedRelease(FGameplayEventData Payload);

	UFUNCTION()
	void PerformHeavyTrace();

	// 跳转到释放攻击段落并启动命中检测
	void ReleaseAttack();

	TArray<TWeakObjectPtr<AActor>> AlreadyHitActors;
	FTimerHandle TraceHandle;

	bool bIsCharging = false;
	bool bReleased = false;
};
