// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_EndDash.generated.h"

/**
 *  用于结束冲刺动画并恢复玩家控制的 AnimNotify。
 */
UCLASS()
class RPG_API UAnimNotify_EndDash : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;
};
