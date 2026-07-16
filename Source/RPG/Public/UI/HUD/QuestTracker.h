// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/HUDWidgetBase.h"
#include "QuestTracker.generated.h"

/**
 * 任务追踪器骨架：列出当前激活任务与其进度
 * 数据源将来自 Quest 子系统（未实现）
 * 暂仅暴露 BP 可调用接口
 */
UCLASS()
class RPG_API UQuestTracker : public UHUDWidgetBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Quest Tracker")
	void AddQuest(FName QuestId, FText DisplayName, int32 Current, int32 Required);

	UFUNCTION(BlueprintCallable, Category="Quest Tracker")
	void UpdateQuest(FName QuestId, int32 Current);

	UFUNCTION(BlueprintCallable, Category="Quest Tracker")
	void CompleteQuest(FName QuestId);

	UFUNCTION(BlueprintCallable, Category="Quest Tracker")
	void ClearQuests();

protected:
	virtual void OnASCReady(UAbilitySystemComponent* ASC) override;
};