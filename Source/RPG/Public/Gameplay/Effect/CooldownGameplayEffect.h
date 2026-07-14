// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "CooldownGameplayEffect.generated.h"

struct FGameplayTag;

/**
 * 冷却效果基类：Duration 类型，持续时长由 SetByCaller 数据标签 Data.Magnitude 提供。
 * 子类在构造函数中调用 AddCooldownTag() 烘焙各自的冷却标签（通过 TargetTagsGameplayEffectComponent），
 * 以满足引擎对 CooldownGameplayEffectClass 必须授予标签的校验。
 */
UCLASS(Abstract)
class RPG_API UCooldownGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UCooldownGameplayEffect();

protected:
	// 通过 TargetTagsGameplayEffectComponent 将冷却标签烘焙到 CDO 的 CachedGrantedTags
	void AddCooldownTag(const FGameplayTag& InTag);
};