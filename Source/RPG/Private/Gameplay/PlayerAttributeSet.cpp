// Copyright Heartape


#include "Gameplay/PlayerAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "UObject/Field.h"
#include "UObject/UnrealType.h"


UPlayerAttributeSet::UPlayerAttributeSet()
{
	SetHealth(100.f);
	SetMaxHealth(100.f);
	SetMana(50.f);
	SetMaxMana(50.f);
}

void UPlayerAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerAttributeSet, Health);
	DOREPLIFETIME(UPlayerAttributeSet, MaxHealth);
	DOREPLIFETIME(UPlayerAttributeSet, Mana);
	DOREPLIFETIME(UPlayerAttributeSet, MaxMana);
}

void UPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// Health 钳制到 [0, MaxHealth]
	if (Data.EvaluatedData.Attribute == GetHealthAttribute() ||
		Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}

	// Mana 钳制到 [0, MaxMana]
	if (Data.EvaluatedData.Attribute == GetManaAttribute() ||
		Data.EvaluatedData.Attribute == GetMaxManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
}

FGameplayAttribute UPlayerAttributeSet::GetHealthAttribute()
{
	static FGameplayAttribute Attribute(
		FindFProperty<FStructProperty>(UPlayerAttributeSet::StaticClass(), "Health"));
	return Attribute;
}

FGameplayAttribute UPlayerAttributeSet::GetMaxHealthAttribute()
{
	static FGameplayAttribute Attribute(
		FindFProperty<FStructProperty>(UPlayerAttributeSet::StaticClass(), "MaxHealth"));
	return Attribute;
}

FGameplayAttribute UPlayerAttributeSet::GetManaAttribute()
{
	static FGameplayAttribute Attribute(
		FindFProperty<FStructProperty>(UPlayerAttributeSet::StaticClass(), "Mana"));
	return Attribute;
}

FGameplayAttribute UPlayerAttributeSet::GetMaxManaAttribute()
{
	static FGameplayAttribute Attribute(
		FindFProperty<FStructProperty>(UPlayerAttributeSet::StaticClass(), "MaxMana"));
	return Attribute;
}

void UPlayerAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Health, OldHealth);
}

void UPlayerAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, MaxHealth, OldMaxHealth);
}

void UPlayerAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Mana, OldMana);
}

void UPlayerAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, MaxMana, OldMaxMana);
}
