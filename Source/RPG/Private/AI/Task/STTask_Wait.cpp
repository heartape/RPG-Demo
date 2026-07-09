// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/STTask_Wait.h"

#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FSTTask_Wait::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	auto& [WaitTime, ElapsedTime] = Context.GetInstanceData(*this);
	ElapsedTime = 0.f;
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTTask_Wait::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	auto& [WaitTime, ElapsedTime] = Context.GetInstanceData(*this);
	ElapsedTime += DeltaTime;
	if (ElapsedTime >= WaitTime)
	{
		return EStateTreeRunStatus::Succeeded;
	}

	return EStateTreeRunStatus::Running;
}
