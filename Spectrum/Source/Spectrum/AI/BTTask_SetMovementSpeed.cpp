// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_SetMovementSpeed.h"
#include "AIController.h"
#include "Interface/SPCharacterAIInterface.h"

EBTNodeResult::Type UBTTask_SetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	ISPCharacterAIInterface* AIPawn = Cast<ISPCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	AIPawn->SetMovementSpeed(Speed);

	return EBTNodeResult::Succeeded;

}
