// Copyright Heartape

#include "UI/HUD/Minimap.h"

#include "AbilitySystemComponent.h"


void UMinimap::OnASCReady(UAbilitySystemComponent* ASC)
{
	Super::OnASCReady(ASC);
	// Minimap 与 ASC 无直接依赖；保留钩子用于后续如玩家已绑定就绪的 Tick 同步
}

void UMinimap::UpdateMinimap(FVector2D PlayerNormalizedPosition, float PlayerYaw)
{
	// TODO: 在 BP 中实现图标位置更新
}

void UMinimap::AddInterestPoint(FName PointId, FVector2D Position)
{
	// TODO: 在 BP / C++ 中添加兴趣点条目
}

void UMinimap::RemoveInterestPoint(FName PointId)
{
	// TODO: 在 BP / C++ 中移除兴趣点条目
}