// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_SetStateAsPassive.h"

#include "SPAIController.h"

EBTNodeResult::Type UBTTask_SetStateAsPassive::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ASPAIController* AIController = Cast<ASPAIController>(OwnerComp.GetAIOwner());
	if(AIController)
	{
		AIController->SetStatePassvie();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
