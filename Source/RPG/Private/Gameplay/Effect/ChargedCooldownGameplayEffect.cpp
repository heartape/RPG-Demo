// Copyright Heartape


#include "Gameplay/Effect/ChargedCooldownGameplayEffect.h"

#include "Gameplay/Ability.h"


UChargedCooldownGameplayEffect::UChargedCooldownGameplayEffect()
{
	AddCooldownTag(Ability::Cooldown::Charged);
}