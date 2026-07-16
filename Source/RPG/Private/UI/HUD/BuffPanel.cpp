// Copyright Heartape

#include "UI/HUD/BuffPanel.h"

#include "AbilitySystemComponent.h"


void UBuffPanel::OnASCReady(UAbilitySystemComponent* ASC)
{
	Super::OnASCReady(ASC);
	// TODO: 订阅 ASC 的 GenericGameplayEventCallbacks / Tag 事件以驱动 Buff 列表
}

void UBuffPanel::AddBuff(FGameplayTag BuffTag)
{
	// TODO: 把 Buff 加入列表并在 BP 中刷新
}

void UBuffPanel::RemoveBuff(FGameplayTag BuffTag)
{
	// TODO: 移除列表中对应 Buff 并刷新
}

void UBuffPanel::ClearBuffs()
{
	// TODO: 清空 Buff 列表
}