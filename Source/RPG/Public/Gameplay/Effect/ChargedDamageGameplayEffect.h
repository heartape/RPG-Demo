// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "ChargedDamageGameplayEffect.generated.h"


/**
 * 重击伤害效果：Instant，加法修改 Health，倍率由 SetByCaller 数据标签 Data.Magnitude 提供。
 */
UCLASS()
class RPG_API UChargedDamageGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UChargedDamageGameplayEffect();
};