// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Task/STTask_RPGBase.h"
#include "STTask_MoveToPatrolPoint.generated.h"

USTRUCT()
struct FRPGMoveToPatrolPointInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Parameter")
	float AcceptanceRadius = 50.f;

	UPROPERTY()
	bool bSawMoving = false;
};

USTRUCT(meta=(DisplayName="Move To Patrol Point"))
struct RPG_API FSTTask_MoveToPatrolPoint : public FSTTask_RPGBase
{
	GENERATED_BODY()

	using FInstanceDataType = FRPGMoveToPatrolPointInstanceData;

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
