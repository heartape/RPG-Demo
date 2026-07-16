// Copyright Heartape

#include "UI/HUD/QuestTracker.h"

#include "AbilitySystemComponent.h"


void UQuestTracker::OnASCReady(UAbilitySystemComponent* ASC)
{
	Super::OnASCReady(ASC);
	// Quest 子系统当前没有 ASC 依赖；钩子保留
}

void UQuestTracker::AddQuest(FName QuestId, FText DisplayName, int32 Current, int32 Required)
{
	// TODO: 加入任务条目并在 BP 中渲染
}

void UQuestTracker::UpdateQuest(FName QuestId, int32 Current)
{
	// TODO: 更新条目进度
}

void UQuestTracker::CompleteQuest(FName QuestId)
{
	// TODO: 标记完成动画 / 移除条目
}

void UQuestTracker::ClearQuests()
{
	// TODO: 清空任务列表
}