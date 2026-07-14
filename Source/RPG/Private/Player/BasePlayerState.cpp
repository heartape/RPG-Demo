// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BasePlayerState.h"

#include "AbilitySystemComponent.h"
#include "Gameplay/PlayerAttributeSet.h"

ABasePlayerState::ABasePlayerState()
{
	SetNetUpdateFrequency(100.f);

	// 创建 ASC 作为 PlayerState 的子对象，使用 Mixed 复制模式
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// 属性集：作为 PlayerState 默认子对象，ASC 在 InitAbilityActorInfo 时会扫描并注册
	AttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>("CharacterAttributeSet");
}

UAbilitySystemComponent* ABasePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}