// Copyright Heartape

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "PlayerAttributeSet.generated.h"

// 手写属性访问器声明宏替代品：避免依赖 ATTRIBUTE_ACCESSORS 的 FindFProperty 路径
#define UXA_ATTRIBUTE_GETTER(PropertyName) \
	float Get##PropertyName() const { return PropertyName.GetCurrentValue(); }

#define UXA_ATTRIBUTE_SETTER(PropertyName) \
	void Set##PropertyName(float NewVal) { PropertyName.SetCurrentValue(NewVal); }

#define UXA_ATTRIBUTE_INITTER(PropertyName) \
	void Init##PropertyName(float NewVal) { PropertyName.SetBaseValue(NewVal); PropertyName.SetCurrentValue(NewVal); }

/**
 * 角色核心属性集：Health / MaxHealth / Mana / MaxMana。
 * 由 ASC 所在的 Actor（PlayerState / EnemyCharacter）持有并以默认子对象形式注册到 ASC。
 */
UCLASS(BlueprintType)
class RPG_API UPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPlayerAttributeSet();

	// 属性复制
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;

	UXA_ATTRIBUTE_GETTER(Health);
	UXA_ATTRIBUTE_SETTER(Health);
	UXA_ATTRIBUTE_INITTER(Health);

	UXA_ATTRIBUTE_GETTER(MaxHealth);
	UXA_ATTRIBUTE_SETTER(MaxHealth);
	UXA_ATTRIBUTE_INITTER(MaxHealth);

	UXA_ATTRIBUTE_GETTER(Mana);
	UXA_ATTRIBUTE_SETTER(Mana);
	UXA_ATTRIBUTE_INITTER(Mana);

	UXA_ATTRIBUTE_GETTER(MaxMana);
	UXA_ATTRIBUTE_SETTER(MaxMana);
	UXA_ATTRIBUTE_INITTER(MaxMana);

	// 获取属性的 FGameplayAttribute 句柄，用于 GE Modifiers 引用属性
	static FGameplayAttribute GetHealthAttribute();
	static FGameplayAttribute GetMaxHealthAttribute();
	static FGameplayAttribute GetManaAttribute();
	static FGameplayAttribute GetMaxManaAttribute();

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
};