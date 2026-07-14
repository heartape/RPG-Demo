// Copyright Heartape


#include "Animation/AnimNotify_CheckChargedAttack.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Gameplay/Ability.h"

void UAnimNotify_CheckChargedAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// 向所有者的 ASC 派发 ChargedCheck 事件，由蓄力能力内的 WaitGameplayEvent 任务接收
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner()))
	{
		FGameplayEventData Payload;
		Payload.EventTag = Ability::ChargedCheck;
		ASC->HandleGameplayEvent(Ability::ChargedCheck, &Payload);
	}
}

FString UAnimNotify_CheckChargedAttack::GetNotifyName_Implementation() const
{
	return FString("Check Charged Attack");
}
