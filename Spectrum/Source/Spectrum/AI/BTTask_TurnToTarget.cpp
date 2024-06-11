// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TurnToTarget.h"

#include "AIController.h"
#include "SPAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Interface/SPCharacterAIInterface.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		UE_LOG(LogTemp,Log,TEXT("ControllingPawn : Failed"));
		return EBTNodeResult::Failed;
	}

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == TargetPawn)
	{
		UE_LOG(LogTemp,Log,TEXT("TargetPawn : Failed"));
		return EBTNodeResult::Failed;
	}

	ISPCharacterAIInterface* AIPawn = Cast<ISPCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		UE_LOG(LogTemp,Log,TEXT("AIPawn : Failed"));

		return EBTNodeResult::Failed;
	}

	AAIController* AIController = OwnerComp.GetAIOwner(); //AIController 가져올 수 있다.
	if(AIController)
	{
		//AActor* TargetActor =UBTFunctionLibrary::GetBlackboardValueAsActor(this,FocusTarget);
		UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET);
		if(TargetObject)
		{
			AActor* TargetActor = Cast<AActor>(TargetObject); 
			AIController->SetFocus(TargetActor,EAIFocusPriority::Gameplay);
		}
		// else
		// {
		// 	FVector Location =  UBTFunctionLibrary::GetBlackboardValueAsVector(this, FocusTarget);
		// 	if(UAIBlueprintHelperLibrary::IsValidAILocation(Location))
		// 	{
		// 		AIController->SetFocalPoint(Location);
		// 	}
		// }
	}
	return EBTNodeResult::Succeeded;     
	//FinishExecute

	// float TurnSpeed = AIPawn->GetAITurnSpeed();
	// FVector LookVector = TargetPawn->GetActorLocation() - ControllingPawn->GetActorLocation();
	// LookVector.Z = 0.0f;
	// FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	// ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), TurnSpeed));

}
