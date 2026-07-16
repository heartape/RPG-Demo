// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/HUDWidgetBase.h"
#include "TargetFrame.generated.h"

class UProgressBar;
class UTextBlock;
class AActor;
class UAbilitySystemComponent;
struct FOnAttributeChangeData;

/**
 * 目标信息面板：显示当前锁定敌方的 HP / Level / Name / Buff
 * 数据源：APlayerCharacter::CurrentTarget -> 其 ASC 上的属性 / 标签
 * Owner（玩家自身）ASC 不直接驱动此 Widget；改为在 SetTargetActor 时挂接到目标 ASC
 */
UCLASS()
class RPG_API UTargetFrame : public UHUDWidgetBase
{
	GENERATED_BODY()

public:
	// 由蓝图 / 玩家角色 SetCurrentTarget 后调用，传入新的目标 Actor
	UFUNCTION(BlueprintCallable, Category="Target Frame")
	void SetTargetActor(AActor* InTarget);

protected:
	virtual void OnASCReady(UAbilitySystemComponent* ASC) override;
	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UTextBlock> NameText;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UTextBlock> LevelText;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UTextBlock> HealthText;

	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UTextBlock> BuffText;

private:
	// 目标使用 UBaseAttributeSet（敌人侧属性集），不同于 UPlayerAttributeSet
	TWeakObjectPtr<AActor> TargetActor;
	TWeakObjectPtr<UAbilitySystemComponent> TargetASC;
	FDelegateHandle TargetHealthHandle;

	void BindTargetASC(UAbilitySystemComponent* InASC);
	void UnbindTargetASC();
	void OnTargetHealthChanged(const FOnAttributeChangeData& Data);
	void RefreshTarget();
};