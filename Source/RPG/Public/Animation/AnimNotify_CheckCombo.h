// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_CheckCombo.generated.h"

/**
 *  用于执行连击字符串检查的 AnimNotify。
 *  如果无法接收到通知，添加通知时选择基于当前c++类的通知可能能解决问题
 */
UCLASS()
class RPG_API UAnimNotify_CheckCombo : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;
};
