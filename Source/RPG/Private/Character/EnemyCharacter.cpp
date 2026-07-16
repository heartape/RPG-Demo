// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"

#include "AbilitySystemComponent.h"
#include "Gameplay/BaseAttributeSet.h"
#include "Abilities/GameplayAbility.h"
#include "Components/WidgetComponent.h"
#include "UI/BaseLifeBar.h"
#include "GameplayEffectTypes.h"


AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent =CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet =CreateDefaultSubobject<UBaseAttributeSet>(TEXT("AttributeSet"));

	LifeBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("LifeBarComponent"));
	LifeBarComponent->SetupAttachment(RootComponent);
	LifeBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
	LifeBarComponent->SetDrawSize(FVector2D(120.f, 30.f));
	LifeBarComponent->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	LifeBarComponent->SetCastShadow(false);
	LifeBarComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AEnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		if (AttributeSet)
		{
			AttributeSet->InitMaxHealth(DefaultMaxHealth);
			AttributeSet->InitHealth(FMath::Clamp(DefaultHealth, 0.f, DefaultMaxHealth));

			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AEnemyCharacter::OnHealthChanged);
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute()).AddUObject(this, &AEnemyCharacter::OnMaxHealthChanged);
		}
		
		if (LifeBarComponent)
		{
			LifeBarWidget = Cast<UBaseLifeBar>(LifeBarComponent->GetUserWidgetObject());
			check(LifeBarWidget);
			UpdateLifeBar();
		}
		
		for (const TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
		{
			if (AbilityClass)
			{
				FGameplayAbilitySpec Spec(AbilityClass, 1, 0, this);
				AbilitySystemComponent->GiveAbility(Spec);
				UE_LOG(LogTemp, Log, TEXT("EnemyCharacter: Granted ability %s"), *AbilityClass->GetName());
			}
		}
		UE_LOG(LogTemp, Log, TEXT("EnemyCharacter: %d abilities granted, total activatable: %d"),
			DefaultAbilities.Num(), AbilitySystemComponent->GetActivatableAbilities().Num());
	}

	if (PatrolPoints.Num() > 0)
	{
		PatrolIndex = 0;
	}
}

void AEnemyCharacter::AdvancePatrolIndex()
{
	if (PatrolPoints.Num() <= 0)
	{
		PatrolIndex = 0;
		return;
	}

	PatrolIndex = (PatrolIndex + 1) % PatrolPoints.Num();
}

void AEnemyCharacter::UpdateLifeBar() const
{
	check(LifeBarWidget)
	check(AttributeSet)

	const float MaxHealth = AttributeSet->GetMaxHealth();
	const float Health = AttributeSet->GetHealth();
	const float Percent = MaxHealth > 0.f ? Health / MaxHealth : 0.f;
	LifeBarWidget->SetLifePercentage(Percent);
}

void AEnemyCharacter::OnHealthChanged(const FOnAttributeChangeData& Data) const
{
	UpdateLifeBar();
}

void AEnemyCharacter::OnMaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	UpdateLifeBar();
}
