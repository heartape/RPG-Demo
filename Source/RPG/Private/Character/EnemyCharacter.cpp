// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"

#include "AbilitySystemComponent.h"
#include "Ability/BaseAttributeSet.h"
#include "Abilities/GameplayAbility.h"


AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent =CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet =CreateDefaultSubobject<UBaseAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AEnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		for (const TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
		{
			if (AbilityClass)
			{
				FGameplayAbilitySpec Spec(AbilityClass, 1, 0, this);
				AbilitySystemComponent->GiveAbility(Spec);
				UE_LOG(LogTemp, Log, TEXT("EnemyCharacter: Granted ability %s"), *AbilityClass->GetName());
			}
		}
		UE_LOG(LogTemp, Log, TEXT("EnemyCharacter: %d abilities granted, total activatable: %d"),
			DefaultAbilities.Num(), AbilitySystemComponent->GetActivatableAbilities().Num());
	}

	if (PatrolPoints.Num() > 0)
	{
		PatrolIndex = 0;
	}
}

void AEnemyCharacter::AdvancePatrolIndex()
{
	if (PatrolPoints.Num() <= 0)
	{
		PatrolIndex = 0;
		return;
	}

	PatrolIndex = (PatrolIndex + 1) % PatrolPoints.Num();
}
