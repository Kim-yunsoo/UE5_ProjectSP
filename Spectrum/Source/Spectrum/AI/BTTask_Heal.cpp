// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Heal.h"
#include "AIController.h"
#include "Character/SPCharacterNonPlayer.h"
#include "Interface/SPCharacterAIInterface.h"


UBTTask_Heal::UBTTask_Heal()
{
}

EBTNodeResult::Type UBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	ASPCharacterNonPlayer* AIPlayer = Cast<ASPCharacterNonPlayer>(ControllingPawn);
	if (nullptr == AIPlayer)
	{
		return EBTNodeResult::Failed;
	}
	FAICharacterHealFinished OnHealFinished;

	OnHealFinished.BindLambda( //원하는 함수를 델리게이트에 바인드
	[&]()
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); //마무리 되었다고 알려주는 함수
	});

	AIPawn->SetAIHealDelegate(OnHealFinished);
	AIPlayer->HealOverTiem();
	return EBTNodeResult::InProgress; 

}
