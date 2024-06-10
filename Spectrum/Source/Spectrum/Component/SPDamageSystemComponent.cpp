// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SPDamageSystemComponent.h"

#include "SpectrumLog.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
USPDamageSystemComponent::USPDamageSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//Health = 100;
	MaxHealth = 100;
	Health = MaxHealth;
	IsDead = false;
	IsInterruptible = true;
	bWantsInitializeComponent = true;
}


// Called when the game starts
void USPDamageSystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USPDamageSystemComponent::InitializeComponent()
{
	SetIsReplicated(true);
	Super::InitializeComponent();
	SetHp(MaxHealth);
}

void USPDamageSystemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(USPDamageSystemComponent, Health);
}

void USPDamageSystemComponent::SetHp(float NewHp)
{
	Health = FMath::Clamp<float>(NewHp, 0.0f, MaxHealth);
	OnHpChanged.Broadcast(Health);
}

float USPDamageSystemComponent::Heal(float Amount)
{
	if (!IsDead)
	{
		Health = Health + Amount;
		Health = FMath::Clamp(Health, 0.0f, MaxHealth);
	}
	return Health;
}

bool USPDamageSystemComponent::TakeDamage(float Amount, bool ShouldForceInterrupt)
{
	if (!IsDead)
	{
		SP_SUBLOG(LogSPNetwork, Log, TEXT("Amount : %f"),Amount);

		Health = Health - Amount;
		SP_SUBLOG(LogSPNetwork, Log, TEXT("Health : %f"),Health);
		SetHp(Health); // HP 위젯 체력 조절 
		if (Health <= 0.0f)
		{
			IsDead = true;
			OnHpZero.Broadcast();
			return true;
		}
		//데미지에 대한 반응
		if (IsInterruptible)
		{
			OnDamageResponse.Broadcast();
		}
	}
	return true;
}
