// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class APatrolPoint;
class UBaseAttributeSet;
class UGameplayAbility;

UCLASS()
class RPG_API AEnemyCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void BeginPlay() override;

	const TArray<TObjectPtr<APatrolPoint>>& GetPatrolPoints() const
	{
		return PatrolPoints;
	}

	int32 GetPatrolIndex() const { return PatrolIndex; }
	void SetPatrolIndex(int32 NewIndex) { PatrolIndex = NewIndex; }
	void AdvancePatrolIndex();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ability")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ability", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBaseAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="AI|Patrol")
	TArray<TObjectPtr<APatrolPoint>> PatrolPoints;

	UPROPERTY(BlueprintReadOnly, Category="AI|Patrol")
	int32 PatrolIndex = 0;
};
