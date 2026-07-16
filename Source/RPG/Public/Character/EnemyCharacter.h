// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class APatrolPoint;
class UBaseAttributeSet;
class UBaseLifeBar;
class UWidgetComponent;
class UGameplayAbility;

struct FOnAttributeChangeData;

UCLASS()
class RPG_API AEnemyCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess=true))
	TObjectPtr<UWidgetComponent> LifeBarComponent;

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

	UFUNCTION(BlueprintCallable, Category="Life Bar")
	void UpdateLifeBar() const;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Life Bar")
	FLinearColor LifeBarColor = FLinearColor::Red;

	UPROPERTY(EditAnywhere, Category="Life Bar")
	TObjectPtr<UBaseLifeBar> LifeBarWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes", meta=(ClampMin="1"))
	float DefaultMaxHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes", meta=(ClampMin="0"))
	float DefaultHealth = 100.f;

private:

	void OnHealthChanged(const FOnAttributeChangeData& Data) const;
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data) const;

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
