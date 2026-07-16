// Copyright Heartape

#include "UI/HUD/TargetFrame.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Character/PlayerCharacter.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Gameplay/BaseAttributeSet.h"


void UTargetFrame::OnASCReady(UAbilitySystemComponent* ASC)
{
	Super::OnASCReady(ASC);

	// Owner ASC 就绪意味着玩家 HUD 已可工作；初始尝试从 PlayerCharacter 取当前目标
	APlayerCharacter* Character = GetOwnerCharacter();
	if (Character)
	{
		SetTargetActor(Character->GetCurrentTarget());
	}
}

void UTargetFrame::SetTargetActor(AActor* InTarget)
{
	if (TargetActor.Get() == InTarget)
	{
		return;
	}

	// 解绑旧目标的 ASC
	UnbindTargetASC();
	TargetActor = InTarget;

	if (InTarget)
	{
		if (UAbilitySystemComponent* NewASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InTarget))
		{
			BindTargetASC(NewASC);
		}
	}

	RefreshTarget();
}

void UTargetFrame::BindTargetASC(UAbilitySystemComponent* InASC)
{
	TargetASC = InASC;
	InASC->GetGameplayAttributeValueChangeDelegate(UBaseAttributeSet::GetHealthAttribute()).RemoveAll(this);
	TargetHealthHandle = InASC->GetGameplayAttributeValueChangeDelegate(UBaseAttributeSet::GetHealthAttribute())
		.AddUObject(this, &UTargetFrame::OnTargetHealthChanged);
}

void UTargetFrame::UnbindTargetASC()
{
	if (UAbilitySystemComponent* OldASC = TargetASC.Get())
	{
		OldASC->GetGameplayAttributeValueChangeDelegate(UBaseAttributeSet::GetHealthAttribute()).RemoveAll(this);
	}
	TargetASC.Reset();
	TargetHealthHandle.Reset();
}

void UTargetFrame::OnTargetHealthChanged(const FOnAttributeChangeData& Data)
{
	RefreshTarget();
}

void UTargetFrame::RefreshTarget()
{
	AActor* Target = TargetActor.Get();
	if (!Target || !TargetHealthHandle.IsValid() || !TargetASC.Get())
	{
		// 无目标：隐藏控件并清空文本
		if (NameText) NameText->SetText(FText::GetEmpty());
		if (LevelText) LevelText->SetText(FText::GetEmpty());
		if (HealthText) HealthText->SetText(FText::GetEmpty());
		if (HealthBar) HealthBar->SetPercent(0.0f);
		return;
	}

	if (NameText)
	{
		NameText->SetText(FText::FromName(Target->GetFName()));
	}

	if (UAbilitySystemComponent* ASC = TargetASC.Get())
	{
		if (const UBaseAttributeSet* AS = ASC->GetSet<UBaseAttributeSet>())
		{
			const float HP = AS->GetHealth();
			const float MaxHP = AS->GetMaxHealth();
			if (HealthBar)
			{
				HealthBar->SetPercent(MaxHP > 0.0f ? FMath::Clamp(HP / MaxHP, 0.0f, 1.0f) : 0.0f);
			}
			if (HealthText)
			{
				HealthText->SetText(FText::Format(
					FText::FromString(TEXT("{0} / {1}")),
					FText::AsNumber(FMath::RoundToInt(HP)),
					FText::AsNumber(FMath::RoundToInt(MaxHP))));
			}
		}
	}

	if (LevelText)
	{
		LevelText->SetText(FText::FromString(TEXT("Lv 1")));
	}
}

void UTargetFrame::NativeDestruct()
{
	UnbindTargetASC();
	Super::NativeDestruct();
}