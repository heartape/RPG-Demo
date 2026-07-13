// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FRPGGameplayTags
{
public:
	static const FRPGGameplayTags& Get();

	static void InitializeNativeTags();

	FGameplayTag State_Attacking;
	FGameplayTag Ability_Attack_Melee;
	FGameplayTag Event_AttackHit;

private:
	FRPGGameplayTags() = default;
	static FRPGGameplayTags& GetInternal();
};
