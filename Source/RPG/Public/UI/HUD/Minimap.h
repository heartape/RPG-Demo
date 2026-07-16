// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/HUDWidgetBase.h"
#include "Minimap.generated.h"

/**
 * 小地图骨架：显示玩家及兴趣点位置
 * 数据源将来自 MinimapVolume / 侦察点 Actor 集合（未实现）
 * 暂仅暴露 BP 可调用接口
 */
UCLASS()
class RPG_API UMinimap : public UHUDWidgetBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Minimap")
	void UpdateMinimap(FVector2D PlayerNormalizedPosition, float PlayerYaw);

	UFUNCTION(BlueprintCallable, Category="Minimap")
	void AddInterestPoint(FName PointId, FVector2D Position);

	UFUNCTION(BlueprintCallable, Category="Minimap")
	void RemoveInterestPoint(FName PointId);

protected:
	virtual void OnASCReady(UAbilitySystemComponent* ASC) override;
};