// Copyright Heartape


#include "Character/BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// 确保网格动画持续更新以支持网络预测
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}

// 基类无 ASC — 由子类提供
UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return nullptr;
}

void ABaseCharacter::GiveStartupAbilities()
{
	if (IsValid(GetAbilitySystemComponent()))
	{
		for (const auto& Ability : StartupAbilities)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability);
			GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
		}
	}
}

