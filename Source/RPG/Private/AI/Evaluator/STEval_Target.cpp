// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Evaluator/STEval_Target.h"

#include "Character/EnemyCharacter.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "StateTreeExecutionContext.h"
#include "NavigationSystem.h"

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

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Context.GetWorld());
	if (!NavSys)
	{
		return;
	}

	FNavLocation EnemyProjected;
	bool bEnemyOnNavMesh = NavSys->ProjectPointToNavigation(Enemy->GetActorLocation(), EnemyProjected, FVector(200.f));
	if (!bEnemyOnNavMesh)
	{
		return;
	}

	if (EnemyProjected.Location != Enemy->GetActorLocation())
	{
		FVector CorrectedLocation = EnemyProjected.Location;
		// 解决敌人陷入地面的问题
		CorrectedLocation.Z = Enemy->GetActorLocation().Z;
		Enemy->SetActorLocation(CorrectedLocation, false, nullptr, ETeleportType::TeleportPhysics);
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(Context.GetWorld(), 0);
	if (!PlayerPawn)
	{
		return;
	}

	float DistSq = FVector::DistSquared2D(Enemy->GetActorLocation(), PlayerPawn->GetActorLocation());
	if (DistSq <= DetectRadius * DetectRadius)
	{
		FNavLocation PlayerProjected;
		bool bPlayerOnNavMesh = NavSys->ProjectPointToNavigation(PlayerPawn->GetActorLocation(), PlayerProjected, FVector(200.f));

		if (bPlayerOnNavMesh)
		{
			TargetActor = PlayerPawn;
			bHasTarget = true;

			if (DistSq <= AttackRange * AttackRange)
			{
				bInAttackRange = true;
			}
		}
	}
}
