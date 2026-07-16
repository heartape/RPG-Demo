// Copyright Heartape

#include "UI/HUD/SkillBar.h"

#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Gameplay/Ability.h"


USkillBar::USkillBar()
{
	if (SlotData.Num() < 4)
	{
		SlotData.SetNum(4);
	}
}

void USkillBar::SetSkillSlot(int32 SlotIndex, TSubclassOf<UGameplayAbility> AbilityClass, FGameplayTag CooldownTag)
{
	if (!SlotData.IsValidIndex(SlotIndex))
	{
		return;
	}
	SlotData[SlotIndex] = FSlotData{AbilityClass, CooldownTag};
	RefreshSlot(SlotIndex, false);
}

void USkillBar::OnASCReady(UAbilitySystemComponent* ASC)
{
	Super::OnASCReady(ASC);

	// 默认槽位映射：Combo / Charged / Healing / Dash 与 Ability::Cooldown.* 标签对应
	if (!SlotData[0].CooldownTag.IsValid()) SlotData[0] = {nullptr, Ability::Cooldown::Combo};
	if (!SlotData[1].CooldownTag.IsValid()) SlotData[1] = {nullptr, Ability::Cooldown::Charged};
	if (!SlotData[2].CooldownTag.IsValid()) SlotData[2] = {nullptr, Ability::Cooldown::Healing};
	if (!SlotData[3].CooldownTag.IsValid()) SlotData[3] = {nullptr, Ability::Cooldown::Dash};

	// 订阅每个槽位冷却标签的添加 / 移除事件
	for (int32 Idx = 0; Idx < SlotData.Num(); ++Idx)
	{
		FSlotData& SlotEntry = SlotData[Idx];
		if (SlotEntry.CooldownTag.IsValid())
		{
			auto& Delegate = ASC->RegisterGameplayTagEvent(SlotEntry.CooldownTag, EGameplayTagEventType::NewOrRemoved);
			SlotEntry.TagHandle = Delegate.AddUObject(this, &USkillBar::OnCooldownTagChanged);

			// 首次刷新：标签若已被持有即视为冷却中
			RefreshSlot(Idx, ASC->HasMatchingGameplayTag(SlotEntry.CooldownTag));
		}
	}
}

void USkillBar::NativeDestruct()
{
	if (UAbilitySystemComponent* ASC = GetOwnerASC())
	{
		for (FSlotData& SlotEntry : SlotData)
		{
			if (SlotEntry.CooldownTag.IsValid() && SlotEntry.TagHandle.IsValid())
			{
				ASC->RegisterGameplayTagEvent(SlotEntry.CooldownTag, EGameplayTagEventType::NewOrRemoved).Remove(SlotEntry.TagHandle);
				SlotEntry.TagHandle.Reset();
			}
		}
	}
	Super::NativeDestruct();
}

void USkillBar::OnCooldownTagChanged(const FGameplayTag Tag, int32 Count)
{
	// 确定哪个槽位被触发，并据此刷新
	const UAbilitySystemComponent* ASC = GetOwnerASC();
	if (!ASC)
	{
		return;
	}

	for (int32 Idx = 0; Idx < SlotData.Num(); ++Idx)
	{
		if (SlotData[Idx].CooldownTag == Tag)
		{
			RefreshSlot(Idx, Count > 0);
		}
	}
}

void USkillBar::RefreshSlot(const int32 Idx, const bool bCooling)
{
	TArray<UProgressBar*> Bars = {Skill1Bar, Skill2Bar, Skill3Bar, UltimateBar};
	TArray<UTextBlock*> Texts = {Skill1Text, Skill2Text, Skill3Text, UltimateText};

	if (!Bars.IsValidIndex(Idx))
	{
		return;
	}

	if (UProgressBar* Bar = Bars[Idx])
	{
		Bar->SetPercent(bCooling ? 0.0f : 1.0f);
	}
	if (UTextBlock* Text = Texts[Idx])
	{
		Text->SetText(FText::FromString(bCooling ? TEXT("CD") : TEXT("OK")));
	}
}