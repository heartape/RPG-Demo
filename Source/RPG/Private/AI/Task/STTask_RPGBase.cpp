// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Task/STTask_RPGBase.h"

#include "AI/BaseAIController.h"
#include "Character/EnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "StateTreeExecutionContext.h"

ABaseAIController* FSTTask_RPGBase::GetAIController(FStateTreeExecutionContext& Context) const
{
	return Cast<ABaseAIController>(Context.GetOwner());
}

AEnemyCharacter* FSTTask_RPGBase::GetEnemyCharacter(FStateTreeExecutionContext& Context) const
{
	ABaseAIController* AIC = GetAIController(Context);
	return AIC ? Cast<AEnemyCharacter>(AIC->GetPawn()) : nullptr;
}

UAbilitySystemComponent* FSTTask_RPGBase::GetASC(FStateTreeExecutionContext& Context) const
{
	AEnemyCharacter* Enemy = GetEnemyCharacter(Context);
	return Enemy ? Enemy->GetAbilitySystemComponent() : nullptr;
}
