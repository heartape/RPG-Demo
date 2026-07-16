// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/HUDWidgetBase.h"
#include "GameHUD.generated.h"

class UPlayerStatus;
class USkillBar;
class UTargetFrame;
class UBuffPanel;
class UMinimap;
class UQuestTracker;
class UNotification;
class UDamageNumber;

/**
 * 游戏战斗 HUD 根容器：在 BP 端以同名子 Widget 放入 Designer 即完成绑定，
 * 在 OnASCReady 时将 Owner 转发给所有子 Widget，使其各自独立地订阅 GAS 数据。
 */
UCLASS()
class RPG_API UGameHUD : public UHUDWidgetBase
{
	GENERATED_BODY()

public:
	// 转发 Owner 到所有子 Widget；GameHUD 自身不订阅 ASC
	virtual void OnASCReady(UAbilitySystemComponent* ASC) override;

protected:
	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UPlayerStatus> PlayerStatus;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<USkillBar> SkillBar;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UTargetFrame> TargetFrame;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UBuffPanel> BuffPanel;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UMinimap> Minimap;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UQuestTracker> QuestTracker;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UNotification> Notification;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UDamageNumber> DamageNumber;
};