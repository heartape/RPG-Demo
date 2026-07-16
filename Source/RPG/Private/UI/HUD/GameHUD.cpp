// Copyright Heartape

#include "UI/HUD/GameHUD.h"

#include "UI/HUD/PlayerStatus.h"
#include "UI/HUD/SkillBar.h"
#include "UI/HUD/TargetFrame.h"
#include "UI/HUD/BuffPanel.h"
#include "UI/HUD/Minimap.h"
#include "UI/HUD/QuestTracker.h"
#include "UI/HUD/Notification.h"
#include "UI/HUD/DamageNumber.h"


void UGameHUD::OnASCReady(UAbilitySystemComponent* ASC)
{
	Super::OnASCReady(ASC);

	APlayerCharacter* Character = GetOwnerCharacter();
	if (!Character)
	{
		return;
	}

	// 转发给所有子 Widget；每个子 Widget 独立完成自己的 GAS 订阅与首次刷新
	if (PlayerStatus)    PlayerStatus->BindToOwner(Character);
	if (SkillBar)         SkillBar->BindToOwner(Character);
	if (TargetFrame)      TargetFrame->BindToOwner(Character);
	if (BuffPanel)       BuffPanel->BindToOwner(Character);
	if (Minimap)         Minimap->BindToOwner(Character);
	if (QuestTracker)    QuestTracker->BindToOwner(Character);
	if (Notification)    Notification->BindToOwner(Character);
	if (DamageNumber)    DamageNumber->BindToOwner(Character);
}