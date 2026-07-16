// Copyright Heartape


#include "Character/PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayEffectTypes.h"
#include "Player/BasePlayerState.h"
#include "Gameplay/Ability.h"
#include "Gameplay/PlayerAttributeSet.h"
#include "TimerManager.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// 初始化平台跳跃状态标记
	bHasWallJumped = false;
	bHasWallJumped = false;
	bHasDoubleJumped = false;
	bIsDead = false;

	// 启用按住跳跃
	JumpMaxHoldTime = 0.4f;

	// 跳跃最大次数设为 3，以支持二段跳与土狼时间判定
	JumpMaxCount = 3;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SpawnTransform = GetActorTransform();
	InitializeAttributes(true);
	BindAttributeDelegates();
}

// 从 PlayerState（权威所有者）获取 ASC
UAbilitySystemComponent* APlayerCharacter::GetAbilitySystemComponent() const
{
	const ABasePlayerState* BasePlayerState = Cast<ABasePlayerState>(GetPlayerState());
	if (IsValid(BasePlayerState))
	{
		return BasePlayerState->GetAbilitySystemComponent();
	}
	return nullptr;
}

// 服务器端：Possessed 后将 ASC 绑定到 PlayerState + Pawn
void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsValid(GetAbilitySystemComponent()) && HasAuthority())
	{
		GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);
		GiveStartupAbilities();
		InitializeAttributes(true);
		BindAttributeDelegates();
	}
}

// 客户端：收到 PlayerState 后重新初始化 ASC 的 Actor 信息
void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (IsValid(GetAbilitySystemComponent()))
	{
		GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);
		BindAttributeDelegates();
	}
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (bIsDead || DamageAmount <= 0.0f || !HasAuthority())
	{
		return 0.0f;
	}

	UPlayerAttributeSet* AttributeSet = GetPlayerAttributeSet();
	if (!AttributeSet)
	{
		return 0.0f;
	}

	const float OldHealth = AttributeSet->GetHealth();
	const float NewHealth = FMath::Clamp(OldHealth - DamageAmount, 0.0f, AttributeSet->GetMaxHealth());
	AttributeSet->SetHealth(NewHealth);

	if (NewHealth <= 0.0f)
	{
		HandleDeath();
	}

	return OldHealth - NewHealth;
}

void APlayerCharacter::DoJumpStart()
{
	if (bIsDead)
	{
		return;
	}

	// 进入多段跳判定
	MultiJump();
}

void APlayerCharacter::DoJumpEnd()
{
	// 停止跳跃
	StopJumping();
}

void APlayerCharacter::MultiJump()
{
	// 冲刺期间忽略跳跃：查询 ASC 上的 State.Dashing 标签（由 DashGameplayAbility 施加）
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if (ASC->HasMatchingGameplayTag(Ability::State::Dashing))
		{
			return;
		}
	}

	// 是否在空中？
	if (GetCharacterMovement()->IsFalling())
	{
		// 是否尚未蹬墙跳？
		if (!bHasWallJumped)
		{
			// 用球体扫掠检测前方是否有墙
			FHitResult OutHit;

			const FVector TraceStart = GetActorLocation();
			const FVector TraceEnd = TraceStart + (GetActorForwardVector() * WallJumpTraceDistance);
			const FCollisionShape TraceShape = FCollisionShape::MakeSphere(WallJumpTraceRadius);

			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this);

			if (GetWorld()->SweepSingleByChannel(OutHit, TraceStart, TraceEnd, FQuat(), ECollisionChannel::ECC_Visibility, TraceShape, QueryParams))
			{
				// 旋转角色背对墙面，为下一次蹬墙跳正确朝向
				FRotator WallOrientation = OutHit.ImpactNormal.ToOrientationRotator();
				WallOrientation.Pitch = 0.0f;
				WallOrientation.Roll = 0.0f;

				SetActorRotation(WallOrientation);

				// 施加弹开 + 向上的冲量完成蹬墙跳
				const FVector WallJumpImpulse = (OutHit.ImpactNormal * WallJumpBounceImpulse) + (FVector::UpVector * WallJumpVerticalImpulse);

				LaunchCharacter(WallJumpImpulse, true, true);

				// 启用拖尾
				SetJumpTrailState(true);

				// 置位蹬墙跳标记以阻止立即二次蹬墙跳
				bHasWallJumped = true;

				GetWorld()->GetTimerManager().SetTimer(WallJumpTimer, this, &APlayerCharacter::ResetWallJump, DelayBetweenWallJumps, false);
			}
			// 没有墙，尝试二段跳
			else
			{
				// 仍在土狼时间范围内？
				if (GetWorld()->GetTimeSeconds() - LastFallTime < MaxCoyoteTime)
				{
					UE_LOG(LogTemp, Warning, TEXT("Coyote Jump"));

					// 使用 CMC 内置功能完成跳跃
					Jump();

					// 启用拖尾
					SetJumpTrailState(true);
				}
				// 不在土狼时间内
				else
				{
					// 空中仅二段跳一次
					if (!bHasDoubleJumped)
					{
						bHasDoubleJumped = true;

						// 使用 CMC 内置功能完成二段跳
						Jump();

						// 启用拖尾
						SetJumpTrailState(true);
					}
				}
			}
		}
	}
	else
	{
		// 在地面则普通跳跃
		Jump();

		// 启用拖尾
		SetJumpTrailState(true);
	}
}

void APlayerCharacter::ResetWallJump()
{
	// 重置蹬墙跳输入锁
	bHasWallJumped = false;
}

bool APlayerCharacter::HasDoubleJumped() const
{
	return bHasDoubleJumped;
}

bool APlayerCharacter::HasWallJumped() const
{
	return bHasWallJumped;
}

float APlayerCharacter::GetRespawnRemainingTime() const
{
	if (!bIsDead || !GetWorld())
	{
		return 0.0f;
	}

	return FMath::Max(0.0f, RespawnEndTime - GetWorld()->GetTimeSeconds());
}

void APlayerCharacter::InitializeAttributes(bool bResetCurrentValues)
{
	if (!HasAuthority())
	{
		return;
	}

	UPlayerAttributeSet* AttributeSet = GetPlayerAttributeSet();
	if (!AttributeSet)
	{
		return;
	}

	const float MaxHealth = FMath::Max(1.0f, DefaultMaxHealth);
	const float MaxMana = FMath::Max(1.0f, DefaultMaxMana);
	AttributeSet->InitMaxHealth(MaxHealth);
	AttributeSet->InitMaxMana(MaxMana);

	if (bResetCurrentValues)
	{
		AttributeSet->InitHealth(FMath::Clamp(DefaultHealth, 0.0f, MaxHealth));
		AttributeSet->InitMana(FMath::Clamp(DefaultMana, 0.0f, MaxMana));
	}
	else
	{
		AttributeSet->SetHealth(FMath::Clamp(AttributeSet->GetHealth(), 0.0f, MaxHealth));
		AttributeSet->SetMana(FMath::Clamp(AttributeSet->GetMana(), 0.0f, MaxMana));
	}
}

void APlayerCharacter::BindAttributeDelegates()
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent();
	if (!AbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetHealthAttribute()).RemoveAll(this);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetHealthAttribute())
		.AddUObject(this, &APlayerCharacter::OnHealthChanged);
}

UPlayerAttributeSet* APlayerCharacter::GetPlayerAttributeSet() const
{
	const ABasePlayerState* BasePlayerState = Cast<ABasePlayerState>(GetPlayerState());
	return BasePlayerState ? BasePlayerState->GetAttributeSet() : nullptr;
}

void APlayerCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (HasAuthority() && Data.NewValue <= 0.0f)
	{
		HandleDeath();
	}
}

void APlayerCharacter::HandleDeath()
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;
	Tags.AddUnique(FName(TEXT("Dead")));
	RespawnEndTime = GetWorld() ? GetWorld()->GetTimeSeconds() + RespawnDelay : 0.0f;

	if (UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent())
	{
		AbilitySystemComponent->CancelAbilities();
	}

	SetJumpTrailState(false);
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetVisibility(false, true);

	if (AController* OwningController = GetController())
	{
		OwningController->SetIgnoreMoveInput(true);
		OwningController->SetIgnoreLookInput(true);
	}

	GetWorldTimerManager().ClearTimer(RespawnTimer);
	GetWorldTimerManager().SetTimer(RespawnTimer, this, &APlayerCharacter::Respawn, RespawnDelay, false);
}

void APlayerCharacter::Respawn()
{
	InitializeAttributes(true);

	SetActorTransform(SpawnTransform, false, nullptr, ETeleportType::TeleportPhysics);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetVisibility(true, true);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	if (AController* OwningController = GetController())
	{
		OwningController->SetIgnoreMoveInput(false);
		OwningController->SetIgnoreLookInput(false);
	}

	Tags.Remove(FName(TEXT("Dead")));
	bIsDead = false;
	RespawnEndTime = 0.0f;
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	// 重置二段跳标记（冲刺使用由 DashGameplayAbility 的冷却管理）
	bHasDoubleJumped = false;

	// 关闭拖尾
	SetJumpTrailState(false);
}

void APlayerCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	// 是否在 falling？
	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling)
	{
		// 记录开始下落的时间，供土狼时间判定使用
		LastFallTime = GetWorld()->GetTimeSeconds();
	}
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// 清理蹬墙跳重置定时器
	GetWorld()->GetTimerManager().ClearTimer(WallJumpTimer);
}
