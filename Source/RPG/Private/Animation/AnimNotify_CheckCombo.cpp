// Copyright Heartape


#include "Animation/AnimNotify_CheckCombo.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Gameplay/Ability.h"

void UAnimNotify_CheckCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// 向所有者的 ASC 派发 ComboCheck 事件，由连击能力内的 WaitGameplayEvent 任务接收
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner()))
	{
		FGameplayEventData Payload;
		Payload.EventTag = Ability::ComboCheck;
		ASC->HandleGameplayEvent(Ability::ComboCheck, &Payload);
	}
}

FString UAnimNotify_CheckCombo::GetNotifyName_Implementation() const
{
	return FString("Check Combo String");
}
