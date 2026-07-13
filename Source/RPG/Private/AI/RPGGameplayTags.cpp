// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/RPGGameplayTags.h"

#include "GameplayTagsManager.h"

const FRPGGameplayTags& FRPGGameplayTags::Get()
{
	return GetInternal();
}

FRPGGameplayTags& FRPGGameplayTags::GetInternal()
{
	static FRPGGameplayTags Tags;
	return Tags;
}

void FRPGGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	FRPGGameplayTags& Tags = GetInternal();

	Tags.State_Attacking = Manager.AddNativeGameplayTag(
		TEXT("State.Attacking"),
		TEXT(""));

	Tags.Ability_Attack_Melee = Manager.AddNativeGameplayTag(
		TEXT("Ability.Attack.Melee"),
		TEXT(""));

	Tags.Event_AttackHit = Manager.AddNativeGameplayTag(
		TEXT("Event.AttackHit"),
		TEXT(""));
}
