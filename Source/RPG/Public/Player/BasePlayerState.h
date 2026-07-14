// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "BasePlayerState.generated.h"

class UPlayerAttributeSet;

/**
 * PlayerState 持有该玩家的 AbilitySystemComponent, 不配置会导致技能无法生效
 */
UCLASS()
class RPG_API ABasePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABasePlayerState();

	// 返回该 PlayerState 持有的 ASC
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

private:
	// 该 PlayerState 拥有的 AbilitySystemComponent 实例
	UPROPERTY(VisibleAnywhere, Category = "Crash|Ability")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// 角色属性集（与 ASC 同 Actor，ASC 在 InitAbilityActorInfo 时自动注册）
	UPROPERTY(VisibleAnywhere, Category = "Crash|Ability")
	TObjectPtr<UPlayerAttributeSet> AttributeSet;
};
