// Copyright Heartape

#include "UI/HUD/DamageNumber.h"

#include "AbilitySystemComponent.h"


void UDamageNumber::OnASCReady(UAbilitySystemComponent* ASC)
{
	Super::OnASCReady(ASC);
	// 后续将监听 ABaseGameMode / AssistComponent 的伤害广播事件
}

void UDamageNumber::SpawnDamageNumber(float Damage, FVector WorldLocation, bool bCritical)
{
	// TODO: 把 WorldLocation 投影到屏幕；创建 Widget 并 AddToViewport；通过动画上浮淡出
}

void UDamageNumber::SpawnHealNumber(float Healing, FVector WorldLocation)
{
	// TODO: 同上但显示绿色治疗值
}