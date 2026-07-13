// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

class UStateTreeAIComponent;
class UStateTree;

UCLASS()
class RPG_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABaseAIController();

	UPROPERTY(EditDefaultsOnly, Category="AI")
	TObjectPtr<UStateTree> StateTreeAsset;

protected:

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	TObjectPtr<UStateTreeAIComponent> StateTreeAIComponent;
};
