// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/HUD/HUDWidgetBase.h"
#include "SkillBar.generated.h"

class UGameplayAbility;
class UProgressBar;
class UTextBlock;
struct FGameplayTag;

/**
 * 技能槽：4 个技能格，绑定 Ability::Cooldown.* 标签查询剩余冷却
 * 数据来源：ASC 已授予能力对应的 Cooldown GameplayEffect 标签
 * 通过 RegisterGameplayTagEvent 监听冷却 GE 标签的添加 / 移除事件，事件驱动刷新
 */
UCLASS()
class RPG_API USkillBar : public UHUDWidgetBase
{
	GENERATED_BODY()

public:
	USkillBar();

	// 第 1..4 槽（Skill1 / Skill2 / Skill3 / Ultimate），由 BP Designer 中绑定 AbilityClass
	UFUNCTION(BlueprintCallable, Category="SkillBar")
	void SetSkillSlot(int32 SlotIndex, TSubclassOf<UGameplayAbility> AbilityClass, FGameplayTag CooldownTag);

protected:
	virtual void OnASCReady(UAbilitySystemComponent* ASC) override;
	virtual void NativeDestruct() override;

private:
	struct FSlotData
	{
		TSubclassOf<UGameplayAbility> AbilityClass;
		FGameplayTag CooldownTag;
		FDelegateHandle TagHandle;
	};

	TArray<FSlotData> SlotData;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UProgressBar> Skill1Bar;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UTextBlock> Skill1Text;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UProgressBar> Skill2Bar;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UTextBlock> Skill2Text;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UProgressBar> Skill3Bar;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UTextBlock> Skill3Text;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UProgressBar> UltimateBar;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UTextBlock> UltimateText;

	void OnCooldownTagChanged(const FGameplayTag Tag, int32 Count);
	void RefreshSlot(int32 Idx, bool bCooling);
};