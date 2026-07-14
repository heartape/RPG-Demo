// Copyright Heartape


#include "Gameplay/Effect/CooldownGameplayEffect.h"

#include "Gameplay/Ability.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"


UCooldownGameplayEffect::UCooldownGameplayEffect()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FSetByCallerFloat SetByCaller;
	SetByCaller.DataTag = Ability::Data::Magnitude;

	DurationMagnitude = FGameplayEffectModifierMagnitude(SetByCaller);
}

void UCooldownGameplayEffect::AddCooldownTag(const FGameplayTag& InTag)
{
	// CreateDefaultSubobject 是构造函数中创建子对象的安全方式；
	// 不能使用引擎的 FindOrAddComponent/AddComponent（内部用 NewObject+NAME_None，在构造函数中会触发致命错误）。
	UTargetTagsGameplayEffectComponent* Component = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("CooldownTargetTagsComponent"));
	if (!GEComponents.Contains(Component))
	{
		GEComponents.Add(Component);
	}

	// 通过组件将冷却标签烘焙到 GE 的 CachedGrantedTags，满足引擎 IsDataValid 校验
	FInheritedTagContainer TagContainer;
	TagContainer.AddTag(InTag);
	Component->SetAndApplyTargetTagChanges(TagContainer);
}