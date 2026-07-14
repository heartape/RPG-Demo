// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Effect/CooldownGameplayEffect.h"
#include "ChargedCooldownGameplayEffect.generated.h"

/**
 * 重击冷却效果：烘焙冷却标签 Cooldown.Ability.Heavy。
 */
UCLASS()
class RPG_API UChargedCooldownGameplayEffect : public UCooldownGameplayEffect
{
	GENERATED_BODY()

public:
	UChargedCooldownGameplayEffect();
};