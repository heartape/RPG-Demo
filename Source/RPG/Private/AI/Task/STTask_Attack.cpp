// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Task/STTask_Attack.h"

#include "AI/RPGGameplayTags.h"
#include "StateTreeExecutionContext.h"
#include "AbilitySystemComponent.h"

EStateTreeRunStatus FSTTask_Attack::EnterState(
	FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	auto& [TimeoutSeconds, bWasAttacking, ElapsedTime] = Context.GetInstanceData(*this);
	bWasAttacking = false;
	ElapsedTime = 0.f;

	UAbilitySystemComponent* ASC = GetASC(Context);
	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("STTask_Attack: ASC is null"));
		return EStateTreeRunStatus::Failed;
	}

	FGameplayTagContainer QueryTags;
	QueryTags.AddTag(FRPGGameplayTags::Get().Ability_Attack_Melee);

	TArray<FGameplayAbilitySpecHandle> MatchingHandles;
	ASC->FindAllAbilitiesWithTags(MatchingHandles, QueryTags);
	UE_LOG(LogTemp, Log, TEXT("STTask_Attack: %d abilities match Ability.Attack.Melee"), MatchingHandles.Num());

	if (MatchingHandles.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("STTask_Attack: No granted ability with tag Ability.Attack.Melee. Check DefaultAbilities on EnemyCharacter and AssetTags on the GA."));
		return EStateTreeRunStatus::Failed;
	}

	if (!ASC->TryActivateAbilitiesByTag(QueryTags))
	{
		UE_LOG(LogTemp, Warning, TEXT("STTask_Attack: TryActivateAbilitiesByTag returned false (ability may be on cooldown or CommitAbility failed)"));
		return EStateTreeRunStatus::Failed;
	}

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTTask_Attack::Tick(
	FStateTreeExecutionContext& Context,
	const float DeltaTime) const
{
	auto& [TimeoutSeconds, bWasAttacking, ElapsedTime] = Context.GetInstanceData(*this);

	UAbilitySystemComponent* ASC = GetASC(Context);
	if (!ASC)
	{
		return EStateTreeRunStatus::Failed;
	}

	ElapsedTime += DeltaTime;

	bool bIsAttacking = ASC->HasMatchingGameplayTag(FRPGGameplayTags::Get().State_Attacking);

	if (bIsAttacking)
	{
		bWasAttacking = true;
		return EStateTreeRunStatus::Running;
	}

	if (bWasAttacking)
	{
		return EStateTreeRunStatus::Succeeded;
	}

	if (ElapsedTime > 0.5f)
	{
		return EStateTreeRunStatus::Failed;
	}

	return EStateTreeRunStatus::Running;
}

void FSTTask_Attack::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	UAbilitySystemComponent* ASC = GetASC(Context);
	if (!ASC)
	{
		return;
	}

	FGameplayTagContainer CancelTags;
	CancelTags.AddTag(FRPGGameplayTags::Get().Ability_Attack_Melee);
	ASC->CancelAbilities(&CancelTags);
}
