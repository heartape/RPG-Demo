// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/HUDWidgetBase.h"
#include "BuffPanel.generated.h"

struct FGameplayTag;

/**
 * Buff 面板骨架：列出玩家身上当前 Gameplay Effect 标签 / Buff 状态
 * 数据源将来自 ASC 的 Active GameplayEffect 及其 GrantedTags
 * 暂仅暴露 BP 可调用接口；订阅 / 渲染逻辑待后续补充
 */
UCLASS()
class RPG_API UBuffPanel : public UHUDWidgetBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Buff Panel")
	void AddBuff(FGameplayTag BuffTag);

	UFUNCTION(BlueprintCallable, Category="Buff Panel")
	void RemoveBuff(FGameplayTag BuffTag);

	UFUNCTION(BlueprintCallable, Category="Buff Panel")
	void ClearBuffs();

protected:
	virtual void OnASCReady(UAbilitySystemComponent* ASC) override;
};