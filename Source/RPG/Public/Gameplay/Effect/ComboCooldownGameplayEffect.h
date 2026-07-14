// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Effect/CooldownGameplayEffect.h"
#include "ComboCooldownGameplayEffect.generated.h"

/**
 * 轻击冷却效果：烘焙冷却标签 Cooldown.Ability.Primary。
 */
UCLASS()
class RPG_API UComboCooldownGameplayEffect : public UCooldownGameplayEffect
{
	GENERATED_BODY()

public:
	UComboCooldownGameplayEffect();
};