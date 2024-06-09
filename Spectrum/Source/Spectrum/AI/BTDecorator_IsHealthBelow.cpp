// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_IsHealthBelow.h"

#include "AIController.h"
#include "Character/SPCharacterNonPlayer.h"

UBTDecorator_IsHealthBelow::UBTDecorator_IsHealthBelow()
{
}

bool UBTDecorator_IsHealthBelow::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	ASPCharacterNonPlayer* AIPlayer = Cast<ASPCharacterNonPlayer>(ControllingPawn);

	if (nullptr == AIPlayer)
	{
		return false;
	}

	float MaxHealth = AIPlayer->GetMaxHealth();
	float CurrentHealth = AIPlayer->GetCurrentHealth();
	float Ratio = CurrentHealth / MaxHealth;
	bResult = (Ratio <= HealthThreshold); // 조건식이 트루면 트루 리턴

	return bResult;
}
