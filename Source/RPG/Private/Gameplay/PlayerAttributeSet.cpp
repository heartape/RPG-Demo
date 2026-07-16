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
	InitHealth(100.f);
	InitMaxHealth(100.f);
	InitMana(50.f);
	InitMaxMana(50.f);
	InitStamina(100.f);
	InitMaxStamina(100.f);
	InitShield(0.f);
	InitMaxShield(100.f);
}

void UPlayerAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerAttributeSet, Health);
	DOREPLIFETIME(UPlayerAttributeSet, MaxHealth);
	DOREPLIFETIME(UPlayerAttributeSet, Mana);
	DOREPLIFETIME(UPlayerAttributeSet, MaxMana);
	DOREPLIFETIME(UPlayerAttributeSet, Stamina);
	DOREPLIFETIME(UPlayerAttributeSet, MaxStamina);
	DOREPLIFETIME(UPlayerAttributeSet, Shield);
	DOREPLIFETIME(UPlayerAttributeSet, MaxShield);
}

void UPlayerAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
	else if (Attribute == GetShieldAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxShield());
	}
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

	// Stamina 钳制到 [0, MaxStamina]
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute() ||
		Data.EvaluatedData.Attribute == GetMaxStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
	}

	// Shield 钳制到 [0, MaxShield]
	if (Data.EvaluatedData.Attribute == GetShieldAttribute() ||
		Data.EvaluatedData.Attribute == GetMaxShieldAttribute())
	{
		SetShield(FMath::Clamp(GetShield(), 0.f, GetMaxShield()));
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

FGameplayAttribute UPlayerAttributeSet::GetStaminaAttribute()
{
	static FGameplayAttribute Attribute(
		FindFProperty<FStructProperty>(UPlayerAttributeSet::StaticClass(), "Stamina"));
	return Attribute;
}

FGameplayAttribute UPlayerAttributeSet::GetMaxStaminaAttribute()
{
	static FGameplayAttribute Attribute(
		FindFProperty<FStructProperty>(UPlayerAttributeSet::StaticClass(), "MaxStamina"));
	return Attribute;
}

FGameplayAttribute UPlayerAttributeSet::GetShieldAttribute()
{
	static FGameplayAttribute Attribute(
		FindFProperty<FStructProperty>(UPlayerAttributeSet::StaticClass(), "Shield"));
	return Attribute;
}

FGameplayAttribute UPlayerAttributeSet::GetMaxShieldAttribute()
{
	static FGameplayAttribute Attribute(
		FindFProperty<FStructProperty>(UPlayerAttributeSet::StaticClass(), "MaxShield"));
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

void UPlayerAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Stamina, OldStamina);
}

void UPlayerAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, MaxStamina, OldMaxStamina);
}

void UPlayerAttributeSet::OnRep_Shield(const FGameplayAttributeData& OldShield) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Shield, OldShield);
}

void UPlayerAttributeSet::OnRep_MaxShield(const FGameplayAttributeData& OldMaxShield) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, MaxShield, OldMaxShield);
}
