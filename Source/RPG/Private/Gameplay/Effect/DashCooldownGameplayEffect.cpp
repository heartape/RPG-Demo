// Copyright Heartape


#include "Gameplay/Effect/DashCooldownGameplayEffect.h"

#include "Gameplay/Ability.h"


UDashCooldownGameplayEffect::UDashCooldownGameplayEffect()
{
	AddCooldownTag(Ability::Cooldown::Dash);
}
