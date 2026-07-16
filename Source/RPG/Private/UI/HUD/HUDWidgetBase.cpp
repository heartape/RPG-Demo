// Copyright Heartape

#include "UI/HUD/HUDWidgetBase.h"

#include "AbilitySystemComponent.h"
#include "Character/PlayerCharacter.h"
#include "Gameplay/PlayerAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"


void UHUDWidgetBase::BindToOwner(APlayerCharacter* InOwner)
{
	if (!InOwner)
	{
		return;
	}

	OwnerCharacter = InOwner;
	TryBindASC();
}

void UHUDWidgetBase::TryBindASC()
{
	if (bASCBound)
	{
		return;
	}

	if (UAbilitySystemComponent* ASC = GetOwnerASC())
	{
		bASCBound = true;
		OnASCReady(ASC);
		return;
	}

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(RetryTimer, this, &UHUDWidgetBase::TryBindASC, 0.1f, false);
	}
}

UAbilitySystemComponent* UHUDWidgetBase::GetOwnerASC() const
{
	APlayerCharacter* Character = OwnerCharacter.Get();
	return Character ? Character->GetAbilitySystemComponent() : nullptr;
}

const UPlayerAttributeSet* UHUDWidgetBase::GetOwnerPlayerAttributeSet() const
{
	APlayerCharacter* Character = OwnerCharacter.Get();
	if (!Character)
	{
		return nullptr;
	}

	if (UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent())
	{
		return ASC->GetSet<UPlayerAttributeSet>();
	}
	return nullptr;
}