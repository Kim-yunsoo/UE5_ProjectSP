// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_ClearFocus.h"

#include "AIController.h"

UBTTask_ClearFocus::UBTTask_ClearFocus()
{
}

EBTNodeResult::Type UBTTask_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	AAIController* AIController = OwnerComp.GetAIOwner(); //AIController 가져올 수 있다.
	if(AIController)
	{
		AIController->ClearFocus(EAIFocusPriority::Default);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
	
}
