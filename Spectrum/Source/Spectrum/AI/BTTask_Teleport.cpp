// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Teleport.h"
#include "AIController.h"
#include "SPAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/SPCharacterNonPlayer.h"
#include "Interface/SPCharacterAIInterface.h"

EBTNodeResult::Type UBTTask_Teleport::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}
	ASPCharacterNonPlayer* AICharacter = Cast<ASPCharacterNonPlayer>(ControllingPawn);
	if(nullptr==AICharacter)
	{
		return EBTNodeResult::Failed;
	}

	ISPCharacterAIInterface* AIPawn = Cast<ISPCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}
	FAICharacterTeleportFinished OnAttackFinished;
	 //델리게이트 선언
	OnAttackFinished.BindLambda( //원하는 함수를 델리게이트에 바인드
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); //마무리 되었다고 알려주는 함수
		}
	);
	FVector Location = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_POINTOFINTEREST);
	AIPawn->SetAITeleportDelegate(OnAttackFinished);
	AICharacter->Teleport(Location);
	return EBTNodeResult::InProgress;
}
