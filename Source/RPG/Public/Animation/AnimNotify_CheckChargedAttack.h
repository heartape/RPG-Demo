// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_CheckChargedAttack.generated.h"

/**
 *  用于执行蓄力攻击保持检查的 AnimNotify。
 */
UCLASS()
class RPG_API UAnimNotify_CheckChargedAttack : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;
};
