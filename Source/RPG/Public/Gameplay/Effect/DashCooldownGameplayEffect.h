// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Effect/CooldownGameplayEffect.h"
#include "DashCooldownGameplayEffect.generated.h"

/**
 * 冲刺冷却效果：烘焙冷却标签 Cooldown.Ability.Dash。
 */
UCLASS()
class RPG_API UDashCooldownGameplayEffect : public UCooldownGameplayEffect
{
	GENERATED_BODY()

public:
	UDashCooldownGameplayEffect();
};
