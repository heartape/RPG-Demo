// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/TimerHandle.h"
#include "BasePlayerController.generated.h"

struct FGameplayTag;
class UInputMappingContext;
class UInputAction;
class UGameHUD;
class APlayerCharacter;
struct FInputActionValue;


/**
 *
 */
UCLASS()
class RPG_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Gameplay|UI")
	TSubclassOf<UGameHUD> GameHUDClass;

	UPROPERTY(Transient)
	TObjectPtr<UGameHUD> GameHUD;

	// 在 Pawn 就位 / ASC 就绪期间重试绑定 GameHUD 的定时器
	FTimerHandle HUDBindRetryTimer;

	void CreateGameHUD();
	void BindGameHUDToOwner();
	void TryBindGameHUD();

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Input|Movement")
	TObjectPtr<UInputMappingContext> MovementInputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Gameplay|Input|Movement")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category="Gameplay|Input|Movement")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category="Gameplay|Input|Movement")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Input|Ability")
	TObjectPtr<UInputMappingContext> AbilityInputMappingContext;
	// GAS 能力输入动作占位
	UPROPERTY(EditDefaultsOnly, Category="Gameplay|Input|Ability")
	UInputAction* ComboAction;

	UPROPERTY(EditDefaultsOnly, Category="Gameplay|Input|Ability")
	UInputAction* ChargedAction;

	UPROPERTY(EditDefaultsOnly, Category="Gameplay|Input|Ability")
	UInputAction* DashAction;

	void Jump();
	void StopJumping();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void Combo();
	void Charged();
	void ChargedRelease();
	void Dash();

	// 以 GameplayEvent 激活由事件触发的能力（Combo / Charged / ChargedRelease / Dash）
	void SendAbilityEvent(const FGameplayTag& GameplayTag) const;
	// 以标签激活能力（Healing 等无需中途输入的能力）
	void ActivateAbility(const FGameplayTag& GameplayTag) const;
};
