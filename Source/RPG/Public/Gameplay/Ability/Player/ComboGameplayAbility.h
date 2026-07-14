// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "BaseGameplayAbility.h"
#include "ComboGameplayAbility.generated.h"

class UAnimMontage;
class UGameplayEffect;
struct FGameplayEventData;

/**
 * 连击攻击能力：由 Ability.Combo 事件激活。
 * - 播放攻击蒙太奇并在出手帧附近做球体检测，将伤害 GE 施加到命中目标
 * - 连击字符串：连击中的再次按键经 Ability.Combo 事件缓存输入；
 *   每段末尾由 AnimNotify_CheckCombo 发送 Ability.ComboCheck 事件触发段落推进
 */
UCLASS()
class RPG_API UComboGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UComboGameplayAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	// 攻击蒙太奇
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Ability")
	TObjectPtr<UAnimMontage> AttackMontage;

	// 各连击阶段对应的蒙太奇段落名（为空则退化为单段攻击）
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Ability|Combo")
	TArray<FName> ComboSectionNames = {"Melee01", "Melee02", "Melee03"};

	// 伤害效果类（应修改 "Vital.Health" 属性，倍率通过 "Data.Magnitude" SetByCaller 获取）
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Ability")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay|Ability", meta = (ClampMin = "0.0"))
	float DamageMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay|Ability", meta = (ClampMin = "0.0"))
	float TraceRadius = 30.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay|Ability", meta = (ClampMin = "0.0"))
	float TraceRange = 200.0f;

	// 出手后做多帧 sphere trace 的时间窗口（秒）—— 在此窗口内每个命中 Actor 仅判定一次
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Ability", meta = (ClampMin = "0.0"))
	float HitWindow = 0.25f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Ability", meta = (ClampMin = "0.0"))
	float TraceInterval = 0.05f;

private:
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// 连击中的再次按键事件回调：缓存输入状态
	UFUNCTION()
	void OnComboInput(FGameplayEventData Payload);

	// 连击段落推进检查事件回调（由 AnimNotify_CheckCombo 发送）
	UFUNCTION()
	void OnComboCheck(FGameplayEventData Payload);

	UFUNCTION()
	void PerformTrace();

	// 启动命中检测时间窗口
	void StartTrace();
	// 停止命中检测
	void StopTrace();

	TArray<TWeakObjectPtr<AActor>> AlreadyHitActors;
	FTimerHandle TraceTimerHandle;
	FTimerHandle StopTraceHandle;

	// 当前连击阶段索引
	int32 ComboCount = 0;

	// 是否有未消费的连击输入
	bool bHasBufferedInput = false;

	// 忽略激活当帧广播的首个事件，避免激活输入被误判为连击推进
	bool bIgnoreFirstInput = false;
};
