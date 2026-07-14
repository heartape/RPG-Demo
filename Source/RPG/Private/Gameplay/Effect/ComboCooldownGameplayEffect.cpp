// Copyright Heartape


#include "Gameplay/Effect/ComboCooldownGameplayEffect.h"

#include "Gameplay/Ability.h"


UComboCooldownGameplayEffect::UComboCooldownGameplayEffect()
{
	AddCooldownTag(Ability::Cooldown::Combo);
}