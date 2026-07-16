// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/HUDWidgetBase.h"
#include "Notification.generated.h"

/**
 * 通知中心骨架：弹幕提示（任务完成 / 受伤 / 拾取）
 * 数据源将来自 Notification 派发器（未实现）
 */
UCLASS()
class RPG_API UNotification : public UHUDWidgetBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Notification")
	void PushNotification(FText Message, float DisplayDuration = 3.0f);

	UFUNCTION(BlueprintCallable, Category="Notification")
	void ClearNotifications();

protected:
	virtual void OnASCReady(UAbilitySystemComponent* ASC) override;
};