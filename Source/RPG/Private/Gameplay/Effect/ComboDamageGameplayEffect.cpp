// Copyright Heartape


#include "Gameplay/Effect/ComboDamageGameplayEffect.h"

#include "Gameplay/PlayerAttributeSet.h"
#include "Gameplay/Ability.h"


UComboDamageGameplayEffect::UComboDamageGameplayEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FSetByCallerFloat SetByCaller;
	SetByCaller.DataTag = Ability::Data::Magnitude;

	FGameplayModifierInfo Modifier;
	Modifier.Attribute = UPlayerAttributeSet::GetHealthAttribute();
	Modifier.ModifierOp = EGameplayModOp::Additive;
	Modifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(SetByCaller);

	Modifiers.Add(Modifier);
}