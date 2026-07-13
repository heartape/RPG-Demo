// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/AnimNotify_AttackHit.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AI/RPGGameplayTags.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_AttackHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp || !MeshComp->GetOwner())
	{
		return;
	}

	AActor* Owner = MeshComp->GetOwner();
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
	if (!ASC)
	{
		return;
	}

	FGameplayEventData EventData;
	EventData.Instigator = Owner;
	EventData.Target = Owner;
	EventData.EventTag = FRPGGameplayTags::Get().Event_AttackHit;

	ASC->HandleGameplayEvent(EventData.EventTag, &EventData);
}

FString UAnimNotify_AttackHit::GetNotifyName_Implementation() const
{
	return TEXT("Attack Hit");
}
