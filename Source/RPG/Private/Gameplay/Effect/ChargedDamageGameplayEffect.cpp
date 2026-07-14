// Copyright Heartape


#include "Gameplay/Effect/ChargedDamageGameplayEffect.h"

#include "Gameplay/PlayerAttributeSet.h"
#include "Gameplay/Ability.h"


UChargedDamageGameplayEffect::UChargedDamageGameplayEffect()
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