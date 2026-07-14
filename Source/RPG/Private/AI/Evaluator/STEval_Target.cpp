// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Evaluator/STEval_Target.h"

#include "Character/EnemyCharacter.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "StateTreeExecutionContext.h"

void FSTEval_Target::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	auto& [DetectRadius, AttackRange, TargetActor, bHasTarget, bInAttackRange] = Context.GetInstanceData(*this);

	TargetActor = nullptr;
	bHasTarget = false;
	bInAttackRange = false;

	AAIController* AIC = Cast<AAIController>(Context.GetOwner());
	AEnemyCharacter* Enemy = AIC ? Cast<AEnemyCharacter>(AIC->GetPawn()) : nullptr;
	if (!Enemy)
	{
		return;
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(Context.GetWorld(), 0);
	if (!PlayerPawn)
	{
		return;
	}

	float DistSq = FVector::DistSquared2D(Enemy->GetActorLocation(), PlayerPawn->GetActorLocation());
	if (DistSq <= DetectRadius * DetectRadius)
	{
		TargetActor = PlayerPawn;
		bHasTarget = true;

		if (DistSq <= AttackRange * AttackRange)
		{
			bInAttackRange = true;
		}
	}
}
