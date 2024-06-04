// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SPNonCharacterStatComponent.h"

// Sets default values for this component's properties
USPNonCharacterStatComponent::USPNonCharacterStatComponent()
{
	MaxHp = 200.0f;
	AttackRadius = 50.0f; 
	CurrentHp = MaxHp;
}

// Called when the game starts
void USPNonCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	SetHp(MaxHp);	
}

float USPNonCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);
	//-���� ������ �ʵ��� ó���Ѵ�. 

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER) //���� ����ϴ� ������ �۴ٸ� ���� ���·� �����Ѵ�. 
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}


void USPNonCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
	
	OnHpChanged.Broadcast(CurrentHp);
}
