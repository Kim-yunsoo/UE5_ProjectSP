// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_CanSeeTarget.h"

#include "AIController.h"

class ISPCharacterAIInterface;

bool UBTDecorator_CanSeeTarget::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult =  Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	
}
