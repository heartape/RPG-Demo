// Copyright Heartape


#include "Player/BasePlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "Gameplay/PlayerAttributeSet.h"
#include "Gameplay/Ability.h"
#include "Character/PlayerCharacter.h"
#include "TimerManager.h"
#include "UI/PlayerStatusWidget.h"

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		CreatePlayerStatusWidget();
		GetWorldTimerManager().SetTimer(
			StatusWidgetRefreshTimer,
			this,
			&ABasePlayerController::RefreshPlayerStatusWidget,
			0.1f,
			true);
		RefreshPlayerStatusWidget();
	}
}

void ABasePlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(StatusWidgetRefreshTimer);
	Super::EndPlay(EndPlayReason);
}

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
	}
}

void ABasePlayerController::CreatePlayerStatusWidget()
{
	if (PlayerStatusWidget || !IsLocalController())
	{
		return;
	}

	check(PlayerStatusWidgetClass)
	PlayerStatusWidget = CreateWidget<UPlayerStatusWidget>(this, PlayerStatusWidgetClass);
	if (PlayerStatusWidget)
	{
		PlayerStatusWidget->AddToViewport();
	}
}

void ABasePlayerController::RefreshPlayerStatusWidget()
{
	if (!PlayerStatusWidget)
	{
		CreatePlayerStatusWidget();
	}

	if (!PlayerStatusWidget)
	{
		return;
	}

	float Health = 0.0f;
	float MaxHealth = 0.0f;
	float Mana = 0.0f;
	float MaxMana = 0.0f;

	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()))
	{
		if (const UPlayerAttributeSet* AttributeSet = AbilitySystemComponent->GetSet<UPlayerAttributeSet>())
		{
			Health = AttributeSet->GetHealth();
			MaxHealth = AttributeSet->GetMaxHealth();
			Mana = AttributeSet->GetMana();
			MaxMana = AttributeSet->GetMaxMana();
		}
	}

	PlayerStatusWidget->SetAttributeValues(Health, MaxHealth, Mana, MaxMana);

	const APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	PlayerStatusWidget->SetRespawnCountdown(PlayerCharacter ? PlayerCharacter->GetRespawnRemainingTime() : 0.0f);
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
