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

	OnHealFinished.BindLambda( //���ϴ� �Լ��� ��������Ʈ�� ���ε�
	[&]()
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); //������ �Ǿ��ٰ� �˷��ִ� �Լ�
	});

	AIPawn->SetAIHealDelegate(OnHealFinished);
	AIPlayer->HealOverTiem();
	return EBTNodeResult::InProgress; 

}
