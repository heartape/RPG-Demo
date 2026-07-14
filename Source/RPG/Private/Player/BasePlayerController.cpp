// Copyright Heartape


#include "Player/BasePlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "Gameplay/Ability.h"
#include "Character/PlayerCharacter.h"

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(Subsystem))
	{
		Subsystem->AddMappingContext(MovementInputMappingContext, 0);
		Subsystem->AddMappingContext(AbilityInputMappingContext, 1);
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABasePlayerController::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ABasePlayerController::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasePlayerController::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABasePlayerController::Look);
		EnhancedInputComponent->BindAction(ComboAction, ETriggerEvent::Started, this, &ABasePlayerController::Combo);
		EnhancedInputComponent->BindAction(ChargedAction, ETriggerEvent::Started, this, &ABasePlayerController::Charged);
		EnhancedInputComponent->BindAction(ChargedAction, ETriggerEvent::Completed, this, &ABasePlayerController::ChargedRelease);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ABasePlayerController::Dash);
		EnhancedInputComponent->BindAction(HealingAction, ETriggerEvent::Started, this, &ABasePlayerController::Healing);
	}
}

void ABasePlayerController::Jump()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetCharacter()))
	{
		PlayerCharacter->DoJumpStart();
	}
	else
	{
		GetCharacter()->Jump();
	}
}

void ABasePlayerController::StopJumping()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetCharacter()))
	{
		PlayerCharacter->DoJumpEnd();
	}
	else
	{
		GetCharacter()->StopJumping();
	}
}

void ABasePlayerController::Move(const FInputActionValue& Value)
{
	
	if (IsValid(GetPawn()))
	{
		// 蹬墙跳后短暂锁定移动输入
		if (const APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
		{
			if (PlayerCharacter->HasWallJumped())
			{
				return;
			}
		}

		const FVector2D MovementVector = Value.Get<FVector2D>();
		const FRotator YawRotation(0, GetControlRotation().Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		GetPawn()->AddMovementInput(ForwardDirection, MovementVector.Y);
		GetPawn()->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ABasePlayerController::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
}

void ABasePlayerController::Combo()
{
	// 以事件激活：未激活时触发能力；激活中时事件广播到能力内的 WaitGameplayEvent 任务以缓存连击输入
	SendAbilityEvent(Ability::Combo);
}

void ABasePlayerController::Charged()
{
	SendAbilityEvent(Ability::Charged);
}

void ABasePlayerController::ChargedRelease()
{
	SendAbilityEvent(Ability::ChargedRelease);
}

void ABasePlayerController::Dash()
{
	SendAbilityEvent(Ability::Dash);
}

void ABasePlayerController::Healing()
{
	ActivateAbility(Ability::Healing);
}

void ABasePlayerController::SendAbilityEvent(const FGameplayTag& GameplayTag) const
{
	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());

	if (IsValid(AbilitySystemComponent))
	{
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTag;
		AbilitySystemComponent->HandleGameplayEvent(GameplayTag, &Payload);
	}
}

void ABasePlayerController::ActivateAbility(const FGameplayTag& GameplayTag) const
{
	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());

	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->TryActivateAbilitiesByTag(GameplayTag.GetSingleTagContainer());
	}
}
