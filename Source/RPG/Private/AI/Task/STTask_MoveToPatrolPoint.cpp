// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Task/STTask_MoveToPatrolPoint.h"

#include "AI/BaseAIController.h"
#include "AI/Patrol/PatrolPoint.h"
#include "Character/EnemyCharacter.h"
#include "StateTreeExecutionContext.h"
#include "Navigation/PathFollowingComponent.h"

EStateTreeRunStatus FSTTask_MoveToPatrolPoint::EnterState(
	FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	auto& [AcceptanceRadius, bSawMoving] = Context.GetInstanceData(*this);
	bSawMoving = false;

	AEnemyCharacter* Enemy = GetEnemyCharacter(Context);
	ABaseAIController* AIC = GetAIController(Context);
	if (!Enemy || !AIC)
	{
		UE_LOG(LogTemp, Error, TEXT("STTask_MoveToPatrolPoint: Missing Enemy or AIController"));
		return EStateTreeRunStatus::Failed;
	}

	const TArray<TObjectPtr<APatrolPoint>>& Points = Enemy->GetPatrolPoints();
	if (Points.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("STTask_MoveToPatrolPoint: No patrol points on %s"), *Enemy->GetName());
		return EStateTreeRunStatus::Failed;
	}

	int32 Index = Enemy->GetPatrolIndex();
	if (!Points.IsValidIndex(Index))
	{
		Index = 0;
		Enemy->SetPatrolIndex(0);
	}

	APatrolPoint* Target = Points[Index];
	Enemy->AdvancePatrolIndex();

	if (!Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("STTask_MoveToPatrolPoint: PatrolPoint[%d] is null"), Index);
		return EStateTreeRunStatus::Failed;
	}

	FAIMoveRequest MoveReq;
	MoveReq.SetGoalActor(Target);
	MoveReq.SetAcceptanceRadius(AcceptanceRadius);
	MoveReq.SetUsePathfinding(true);
	MoveReq.SetAllowPartialPath(true);

	FPathFollowingRequestResult Result = AIC->MoveTo(MoveReq);

	if (Result.Code == EPathFollowingRequestResult::Failed)
	{
		UE_LOG(LogTemp, Error, TEXT("STTask_MoveToPatrolPoint: MoveTo failed (no NavMesh or unreachable?) for %s -> %s"),
			*Enemy->GetName(), *Target->GetName());
		return EStateTreeRunStatus::Failed;
	}

	if (Result.Code == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		return EStateTreeRunStatus::Succeeded;
	}

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTTask_MoveToPatrolPoint::Tick(
	FStateTreeExecutionContext& Context,
	const float DeltaTime) const
{
	auto& [AcceptanceRadius, bSawMoving] = Context.GetInstanceData(*this);

	ABaseAIController* AIC = GetAIController(Context);
	if (!AIC)
	{
		return EStateTreeRunStatus::Failed;
	}

	EPathFollowingStatus::Type Status = AIC->GetMoveStatus();

	if (Status == EPathFollowingStatus::Moving || Status == EPathFollowingStatus::Waiting)
	{
		bSawMoving = true;
		return EStateTreeRunStatus::Running;
	}

	if (Status == EPathFollowingStatus::Idle)
	{
		if (bSawMoving)
		{
			return EStateTreeRunStatus::Succeeded;
		}
		return EStateTreeRunStatus::Running;
	}

	return EStateTreeRunStatus::Running;
}
