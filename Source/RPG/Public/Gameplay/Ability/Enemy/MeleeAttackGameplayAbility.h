// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MeleeAttackGameplayAbility.generated.h"

class UAnimMontage;
class UGameplayEffect;

UCLASS()
class RPG_API UMeleeAttackGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UMeleeAttackGameplayAbility();

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	float HitDetectRadius = 100.f;

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags,
		const FGameplayTagContainer* TargetTags,
		OUT FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void PostLoad() override;

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void HandleAttackHit(const FGameplayEventData Payload);

	void ApplyDamageToTargetsInRadius();
};
