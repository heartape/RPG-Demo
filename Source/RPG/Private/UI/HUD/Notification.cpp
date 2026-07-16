// Copyright Heartape

#include "UI/HUD/Notification.h"

#include "AbilitySystemComponent.h"


void UNotification::OnASCReady(UAbilitySystemComponent* ASC)
{
	Super::OnASCReady(ASC);
	// 通知系统暂无 ASC 依赖；保留钩子
}

void UNotification::PushNotification(FText Message, float DisplayDuration)
{
	// TODO: 在 BP 中把 Message 加入滚动列表并按 DisplayDuration 自动淡出
}

void UNotification::ClearNotifications()
{
	// TODO: 清空通知列表
}