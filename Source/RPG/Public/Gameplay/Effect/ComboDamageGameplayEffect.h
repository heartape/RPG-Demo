// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "ComboDamageGameplayEffect.generated.h"


/**
 * 主攻击（轻击）伤害效果：Instant，加法修改 Health，倍率由 SetByCaller 数据标签 Data.Magnitude 提供。
 * 设计器应以 C++ 类为父创建 BP 资产并配置数值/标签。
 */
UCLASS()
class RPG_API UComboDamageGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UComboDamageGameplayEffect();
};