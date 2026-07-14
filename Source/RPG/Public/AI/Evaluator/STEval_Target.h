// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeEvaluatorBase.h"
#include "STEval_Target.generated.h"

USTRUCT()
struct FRPGTargetEvalInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Parameter")
	float DetectRadius = 800.f;

	UPROPERTY(EditAnywhere, Category="Parameter")
	float AttackRange = 120.f;

	UPROPERTY(EditAnywhere, Category="Output")
	AActor* TargetActor = nullptr;

	UPROPERTY(EditAnywhere, Category="Output")
	bool bHasTarget = false;

	UPROPERTY(EditAnywhere, Category="Output")
	bool bInAttackRange = false;
};

USTRUCT(meta=(DisplayName="Find Target"))
struct RPG_API FSTEval_Target : public FStateTreeEvaluatorCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FRPGTargetEvalInstanceData;

	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}

	virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};
