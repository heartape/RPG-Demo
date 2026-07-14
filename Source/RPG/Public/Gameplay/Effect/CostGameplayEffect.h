// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "CostGameplayEffect.generated.h"


/**
 * 通用消耗效果：Instant 类型，加法修改 Mana，数值由 SetByCaller 数据标签 Data.Magnitude 提供。
 * 能力在 ApplyCost 时传入负数（-CostAmount）以扣减法力。
 */
UCLASS()
class RPG_API UCostGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UCostGameplayEffect();
};