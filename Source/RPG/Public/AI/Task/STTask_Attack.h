// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Task/STTask_RPGBase.h"
#include "STTask_Attack.generated.h"

USTRUCT()
struct FRPGAttackInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Parameter")
	float TimeoutSeconds = 5.f;

	UPROPERTY()
	bool bWasAttacking = false;

	UPROPERTY()
	float ElapsedTime = 0.f;
};

USTRUCT(meta=(DisplayName="Attack"))
struct RPG_API FSTTask_Attack : public FSTTask_RPGBase
{
	GENERATED_BODY()

	using FInstanceDataType = FRPGAttackInstanceData;

	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}

	virtual EStateTreeRunStatus EnterState(
		FStateTreeExecutionContext& Context,
		const FStateTreeTransitionResult& Transition) const override;

	virtual EStateTreeRunStatus Tick(
		FStateTreeExecutionContext& Context,
		const float DeltaTime) const override;
};
