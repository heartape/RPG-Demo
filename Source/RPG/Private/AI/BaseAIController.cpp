// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseAIController.h"

#include "Components/StateTreeAIComponent.h"


ABaseAIController::ABaseAIController()
{
	StateTreeAIComponent = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTreeAIComponent"));
	StateTreeAIComponent->SetStartLogicAutomatically(false);
}

void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 暂不启动 StateTree
}

void ABaseAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

