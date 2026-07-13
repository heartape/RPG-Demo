// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseAIController.h"

#include "Components/StateTreeAIComponent.h"
#include "StateTree.h"


ABaseAIController::ABaseAIController()
{
	StateTreeAIComponent = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTreeAIComponent"));
	StateTreeAIComponent->SetStartLogicAutomatically(false);
}

void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (StateTreeAIComponent && StateTreeAsset)
	{
		StateTreeAIComponent->SetStateTree(StateTreeAsset);
		StateTreeAIComponent->StartLogic();
		UE_LOG(LogTemp, Log, TEXT("BaseAIController: StateTree started for %s"), *InPawn->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseAIController: StateTreeAsset or Component missing for %s"), *InPawn->GetName());
	}
}

void ABaseAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

