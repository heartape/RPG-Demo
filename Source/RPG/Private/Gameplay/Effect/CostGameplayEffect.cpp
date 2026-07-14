// Copyright Heartape


#include "Gameplay/Effect/CostGameplayEffect.h"

#include "Gameplay/PlayerAttributeSet.h"
#include "Gameplay/Ability.h"


UCostGameplayEffect::UCostGameplayEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FSetByCallerFloat SetByCaller;
	SetByCaller.DataTag = Ability::Data::Magnitude;

	FGameplayModifierInfo Modifier;
	Modifier.Attribute = UPlayerAttributeSet::GetManaAttribute();
	Modifier.ModifierOp = EGameplayModOp::Additive;
	Modifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(SetByCaller);

	Modifiers.Add(Modifier);
}