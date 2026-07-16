// Copyright Heartape

#include "UI/HUD/PlayerStatus.h"

#include "AbilitySystemComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Gameplay/PlayerAttributeSet.h"


void UPlayerStatus::OnASCReady(UAbilitySystemComponent* ASC)
{
	Super::OnASCReady(ASC);

	const UPlayerAttributeSet* AttributeSet = ASC->GetSet<UPlayerAttributeSet>();
	if (!AttributeSet)
	{
		return;
	}

	// 重复绑定防重：先移除所有 this 的旧绑定，再统一加新绑定
	TArray<FGameplayAttribute> Attrs = {
		UPlayerAttributeSet::GetHealthAttribute(),
		UPlayerAttributeSet::GetMaxHealthAttribute(),
		UPlayerAttributeSet::GetManaAttribute(),
		UPlayerAttributeSet::GetMaxManaAttribute(),
		UPlayerAttributeSet::GetStaminaAttribute(),
		UPlayerAttributeSet::GetMaxStaminaAttribute(),
		UPlayerAttributeSet::GetShieldAttribute(),
		UPlayerAttributeSet::GetMaxShieldAttribute(),
	};
	for (const FGameplayAttribute& Attr : Attrs)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(Attr).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(Attr).AddUObject(this, &UPlayerStatus::OnAttributeChanged);
	}

	// 同步首次快照并刷新 UI
	CachedHealth = AttributeSet->GetHealth();
	CachedMaxHealth = AttributeSet->GetMaxHealth();
	CachedMana = AttributeSet->GetMana();
	CachedMaxMana = AttributeSet->GetMaxMana();
	CachedStamina = AttributeSet->GetStamina();
	CachedMaxStamina = AttributeSet->GetMaxStamina();
	CachedShield = AttributeSet->GetShield();
	CachedMaxShield = AttributeSet->GetMaxShield();
	RefreshAll();
}

void UPlayerStatus::NativeDestruct()
{
	if (UAbilitySystemComponent* ASC = GetOwnerASC())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetHealthAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetMaxHealthAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetManaAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetMaxManaAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetStaminaAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetMaxStaminaAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetShieldAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetMaxShieldAttribute()).RemoveAll(this);
	}
	Super::NativeDestruct();
}

void UPlayerStatus::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
	if (!GetOwnerPlayerAttributeSet())
	{
		return;
	}

	const UPlayerAttributeSet* AS = GetOwnerPlayerAttributeSet();
	(void)Data;
	CachedHealth = AS->GetHealth();
	CachedMaxHealth = AS->GetMaxHealth();
	CachedMana = AS->GetMana();
	CachedMaxMana = AS->GetMaxMana();
	CachedStamina = AS->GetStamina();
	CachedMaxStamina = AS->GetMaxStamina();
	CachedShield = AS->GetShield();
	CachedMaxShield = AS->GetMaxShield();
	RefreshAll();
}

void UPlayerStatus::RefreshAll()
{
	auto SetBar = [](UProgressBar* Bar, float Cur, float Max)
	{
		if (Bar)
		{
			Bar->SetPercent(Max > 0.0f ? FMath::Clamp(Cur / Max, 0.0f, 1.0f) : 0.0f);
		}
	};

	auto SetText = [](UTextBlock* Block, const TCHAR* Prefix, float Cur, float Max)
	{
		if (Block)
		{
			Block->SetText(FText::Format(
				FText::FromString(FString::Printf(TEXT("%s {0} / {1}"), Prefix)),
				FText::AsNumber(FMath::RoundToInt(Cur)),
				FText::AsNumber(FMath::RoundToInt(Max))));
		}
	};

	SetBar(HealthBar, CachedHealth, CachedMaxHealth);
	SetText(HealthText, TEXT("HP"), CachedHealth, CachedMaxHealth);

	SetBar(ManaBar, CachedMana, CachedMaxMana);
	SetText(ManaText, TEXT("MP"), CachedMana, CachedMaxMana);

	SetBar(StaminaBar, CachedStamina, CachedMaxStamina);
	SetText(StaminaText, TEXT("SP"), CachedStamina, CachedMaxStamina);

	SetBar(ShieldBar, CachedShield, CachedMaxShield);
	SetText(ShieldText, TEXT("SH"), CachedShield, CachedMaxShield);
}