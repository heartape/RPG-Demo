// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class APatrolPoint;
class UBaseAttributeSet;

UCLASS()
class RPG_API AEnemyCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	const TArray<TObjectPtr<APatrolPoint>>& GetPatrolPoints() const
	{
		return PatrolPoints;
	}

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ability")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ability", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBaseAttributeSet> AttributeSet;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="AI|Patrol")
	TArray<TObjectPtr<APatrolPoint>> PatrolPoints;
};
