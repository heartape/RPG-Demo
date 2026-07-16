// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Engine/TimerHandle.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class UPlayerAttributeSet;
class USpringArmComponent;
struct FOnAttributeChangeData;

/**
 * 玩家角色：仅承载平台跳跃相关机制（Dash / Combo / Charged 已剥离至 GAS 能力）。
 * - Wall Jump（蹬墙跳）：贴墙时跳跃可借冲击法线反弹
 * - Multi Jump（多段跳）：蹬墙跳 / 土狼时间 / 二段跳
 * - 冲刺期间忽略跳跃：通过查询 ASC 上的 Ability.State.Dashing 标签判定（由 DashGameplayAbility 施加）
 */
UCLASS()
class RPG_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	// 将 ASC 获取委托给所属的 PlayerState
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// 服务器端：在 Possessed 时初始化 ASC 的 Actor 信息
	virtual void PossessedBy(AController* NewController) override;

	// 客户端：PlayerState 同步时初始化 ASC 的 Actor 信息
	virtual void OnRep_PlayerState() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/** 处理跳跃按下输入，进入多段跳判定（可由控制器或 UI 调用） */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpStart();

	/** 处理跳跃松开输入（可由控制器或 UI 调用） */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpEnd();

	/** 重置蹬墙跳输入锁（由定时器调用） */
	void ResetWallJump();

	/** 是否已经二段跳过 */
	UFUNCTION(BlueprintPure, Category = "Platforming")
	bool HasDoubleJumped() const;

	/** 是否已经蹬墙跳过（移动输入期间会短暂锁定） */
	UFUNCTION(BlueprintPure, Category = "Platforming")
	bool HasWallJumped() const;

	UFUNCTION(BlueprintPure, Category = "Combat")
	bool IsDead() const { return bIsDead; }

	UFUNCTION(BlueprintPure, Category = "Combat")
	float GetRespawnRemainingTime() const;

protected:
	virtual void BeginPlay() override;

	// 落地：重置二段跳标记
	virtual void Landed(const FHitResult& Hit) override;

	// 移动模式变化：记录开始下落的时间用于土狼时间
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// 执行多段跳判定（蹬墙跳 / 土狼时间 / 二段跳）
	void MultiJump();

	// 交由蓝图启用 / 关闭跳跃拖尾
	UFUNCTION(BlueprintImplementableEvent, Category = "Platforming", meta = (DisplayName = "Set Jump Trail State"))
	void SetJumpTrailState(bool bEnabled);

	void InitializeAttributes(bool bResetCurrentValues);
	void BindAttributeDelegates();
	UPlayerAttributeSet* GetPlayerAttributeSet() const;
	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void HandleDeath();
	void Respawn();

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditDefaultsOnly, Category="Attributes", meta=(ClampMin="1"))
	float DefaultMaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category="Attributes", meta=(ClampMin="0"))
	float DefaultHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category="Attributes", meta=(ClampMin="1"))
	float DefaultMaxMana = 50.0f;

	UPROPERTY(EditDefaultsOnly, Category="Attributes", meta=(ClampMin="0"))
	float DefaultMana = 50.0f;

	UPROPERTY(EditDefaultsOnly, Category="Combat", meta=(ClampMin="0"))
	float RespawnDelay = 5.0f;

	FTransform SpawnTransform;
	FTimerHandle RespawnTimer;
	float RespawnEndTime = 0.0f;
	uint8 bIsDead : 1;

	// ---- Platforming 状态 ----

	uint8 bHasWallJumped : 1;
	uint8 bHasDoubleJumped : 1;

	// 蹬墙跳输入锁重置定时器
	FTimerHandle WallJumpTimer;

	UPROPERTY(EditAnywhere, Category = "Wall Jump", meta = (ClampMin = 0, ClampMax = 1000, Units = "cm"))
	float WallJumpTraceDistance = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Wall Jump", meta = (ClampMin = 0, ClampMax = 100, Units = "cm"))
	float WallJumpTraceRadius = 25.0f;

	UPROPERTY(EditAnywhere, Category = "Wall Jump", meta = (ClampMin = 0, ClampMax = 10000, Units = "cm/s"))
	float WallJumpBounceImpulse = 800.0f;

	UPROPERTY(EditAnywhere, Category = "Wall Jump", meta = (ClampMin = 0, ClampMax = 10000, Units = "cm/s"))
	float WallJumpVerticalImpulse = 900.0f;

	UPROPERTY(EditAnywhere, Category = "Wall Jump", meta = (ClampMin = 0, ClampMax = 5, Units = "s"))
	float DelayBetweenWallJumps = 0.1f;

	// ---- Coyote Time ----

	float LastFallTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Coyote Time", meta = (ClampMin = 0, ClampMax = 5, Units = "s"))
	float MaxCoyoteTime = 0.16f;
};
