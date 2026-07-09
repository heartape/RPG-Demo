// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "STTask_Wait.generated.h"

USTRUCT()
struct FRPGWaitTaskInstanceData
{
	GENERATED_BODY()

	/** 等待时间（秒） */
	UPROPERTY(EditAnywhere, Category="Parameter")
	float WaitTime = 2.f;

	/** 已等待时间 */
	UPROPERTY()
	float ElapsedTime = 0.f;
};

/**
 * 
 */
USTRUCT(meta=(DisplayName="Wait"))
struct RPG_API FSTTask_Wait : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FRPGWaitTaskInstanceData;

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
