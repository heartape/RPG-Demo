// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/HUDWidgetBase.h"
#include "DamageNumber.generated.h"

/**
 * 浮动伤害数字骨架：在屏幕上 / 目标头上显示伤害值
 * 数据源将来自 TakeDamage 路径或专门的 DamageNumber 组件（未实现）
 */
UCLASS()
class RPG_API UDamageNumber : public UHUDWidgetBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Damage Number")
	void SpawnDamageNumber(float Damage, FVector WorldLocation, bool bCritical = false);

	UFUNCTION(BlueprintCallable, Category="Damage Number")
	void SpawnHealNumber(float Healing, FVector WorldLocation);

protected:
	virtual void OnASCReady(UAbilitySystemComponent* ASC) override;
};