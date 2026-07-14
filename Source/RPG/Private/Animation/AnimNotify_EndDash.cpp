// Copyright Heartape


#include "Animation/AnimNotify_EndDash.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Gameplay/Ability.h"

void UAnimNotify_EndDash::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// 向所有者的 ASC 派发 DashEnd 事件，由冲刺能力内的 WaitGameplayEvent 任务接收以提前恢复重力
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner()))
	{
		FGameplayEventData Payload;
		Payload.EventTag = Ability::DashEnd;
		ASC->HandleGameplayEvent(Ability::DashEnd, &Payload);
	}
}

FString UAnimNotify_EndDash::GetNotifyName_Implementation() const
{
	return FString("End Dash");
}
