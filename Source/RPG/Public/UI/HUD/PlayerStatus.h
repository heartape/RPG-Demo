// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/HUDWidgetBase.h"
#include "PlayerStatus.generated.h"

class UProgressBar;
class UTextBlock;
struct FOnAttributeChangeData;

/**
 * 玩家资源面板：HP / MP / Stamina / Shield 四条
 * 数据全部来自 PlayerAttributeSet；订阅 ASC 的属性变化事件，独立刷新
 */
UCLASS()
class RPG_API UPlayerStatus : public UHUDWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void OnASCReady(UAbilitySystemComponent* ASC) override;
	virtual void NativeDestruct() override;

	// BP 端 Designer 放置的同名控件；BindWidgetOptional 确保缺一不会硬错（用于不同 HUD 风格复用）
	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UTextBlock> HealthText;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UProgressBar> ManaBar;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UTextBlock> ManaText;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UProgressBar> StaminaBar;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UTextBlock> StaminaText;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UProgressBar> ShieldBar;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UTextBlock> ShieldText;

private:
	// 缓存上次显示的属性快照，避免 SetText 反复调度
	float CachedHealth = 0.0f;
	float CachedMaxHealth = 0.0f;
	float CachedMana = 0.0f;
	float CachedMaxMana = 0.0f;
	float CachedStamina = 0.0f;
	float CachedMaxStamina = 0.0f;
	float CachedShield = 0.0f;
	float CachedMaxShield = 0.0f;

	void OnAttributeChanged(const FOnAttributeChangeData& Data);
	void RefreshAll();
};