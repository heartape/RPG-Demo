// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidgetBase.generated.h"

class APlayerCharacter;
class UAbilitySystemComponent;
class UPlayerAttributeSet;

/**
 * HUD 子 Widget 基类：
 * - 仅负责显示，不存放游戏逻辑
 * - 通过 BindToOwner 注入 APlayerCharacter，子类在 OnASCReady 中订阅 GAS 属性 / 标签事件
 * - ASC 尚未就绪时自动按 0.1s 重试，避免依赖控制器的轮询定时器
 */
UCLASS(Abstract)
class RPG_API UHUDWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	// 由 GameHUD 在初始化时调用：保存 Owner 并尝试绑定 ASC
	UFUNCTION(BlueprintCallable, Category="HUD")
	virtual void BindToOwner(APlayerCharacter* InOwner);

protected:
	// ASC 就绪后由子类覆写：注册属性 / 标签事件，并执行首次刷新
	virtual void OnASCReady(UAbilitySystemComponent* ASC) {}

	// Owner getter：子类据此取 ASC / AttributeSet
	APlayerCharacter* GetOwnerCharacter() const { return OwnerCharacter.Get(); }
	UAbilitySystemComponent* GetOwnerASC() const;
	const UPlayerAttributeSet* GetOwnerPlayerAttributeSet() const;

	UPROPERTY(Transient)
	TWeakObjectPtr<APlayerCharacter> OwnerCharacter;

private:
	// 重试 ASC 绑定的定时器句柄
	FTimerHandle RetryTimer;

	uint8 bASCBound : 1 {false};

	void TryBindASC();
};