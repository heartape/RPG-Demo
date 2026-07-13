// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "STTask_RPGBase.generated.h"

class ABaseAIController;
class AEnemyCharacter;
class UAbilitySystemComponent;

USTRUCT(meta=(Hidden))
struct RPG_API FSTTask_RPGBase : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

protected:
	ABaseAIController* GetAIController(FStateTreeExecutionContext& Context) const;
	AEnemyCharacter* GetEnemyCharacter(FStateTreeExecutionContext& Context) const;
	UAbilitySystemComponent* GetASC(FStateTreeExecutionContext& Context) const;
};
