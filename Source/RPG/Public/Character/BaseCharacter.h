// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UGameplayAbility;
// 抽象基类角色，暴露 AbilitySystemComponent 接口
UCLASS(Abstract)
class RPG_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	// 返回 ASC — 由持有 ASC 的子类重写
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	void GiveStartupAbilities();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Ability")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};
