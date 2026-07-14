// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BaseGameplayAbility.generated.h"

class UGameplayEffect;
struct FGameplayTag;

/**
 * GAS RPG 项目的能力基类。
 * 提供通用的能力提交、效果施加、检测等辅助工具，并内置冷却/消耗的 C++ 实现框架。
 * 子类只需在构造函数中配置 CooldownGameplayEffectClass / CooldownDuration / CostAmount 即可生效。
 */
UCLASS(Abstract)
class RPG_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UBaseGameplayAbility();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay|Debug")
	bool bDrawDebug = false;

	// ---- 冷却配置 ----

	// 冷却时长（秒），通过 SetByCaller 注入 CooldownGameplayEffect 的 DurationMagnitude
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay|Ability|Cooldown", meta = (ClampMin = "0.0"))
	float CooldownDuration = 1.0f;

	// ---- 消耗配置 ----

	// 法力消耗量；ApplyCost 时以负数施加到 Mana 属性
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay|Ability|Cost", meta = (ClampMin = "0.0"))
	float CostAmount = 0.0f;

	// ---- 冷却/消耗重写 ----

	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

protected:
	// 提交能力（Cost / Cooldown），失败时结束能力并返回 false
	bool CommitAbilityChecked(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);

	// 在角色前方做球体检测，返回命中 Actor 列表
	TArray<AActor*> SphereTrace(const FGameplayAbilityActorInfo* ActorInfo, float Radius, float Range) const;

	// 将 GE 类施加给目标（带可选的按倍率），倍率通过 SetByCaller "Data.Magnitude" 传入
	void ApplyEffectToActor(const FGameplayAbilityActorInfo* ActorInfo, TSubclassOf<UGameplayEffect> EffectClass, AActor* TargetActor, float Multiplier = 1.0f) const;
};