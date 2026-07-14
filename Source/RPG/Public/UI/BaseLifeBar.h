// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseLifeBar.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UBaseLifeBar : public UUserWidget
{
	GENERATED_BODY()

public:

	/** Sets the life bar to the provided 0-1 percentage value*/
	UFUNCTION(BlueprintImplementableEvent, Category="Life Bar")
	void SetLifePercentage(float Percent);

	// Sets the life bar fill color
	UFUNCTION(BlueprintImplementableEvent, Category="Life Bar")
	void SetBarColor(FLinearColor Color);
};
