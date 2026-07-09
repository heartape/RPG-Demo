// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

class UStateTreeAIComponent;

UCLASS()
class RPG_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABaseAIController();
protected:

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	TObjectPtr<UStateTreeAIComponent> StateTreeAIComponent;
};
