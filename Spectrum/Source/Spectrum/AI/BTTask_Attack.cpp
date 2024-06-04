// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"

#include "AIController.h"
#include "Interface/SPCharacterAIInterface.h"

UBTTask_Attack::UBTTask_Attack()
{
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

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

	FAICharacterAttackFinished OnAttackFinished; //��������Ʈ ����
	OnAttackFinished.BindLambda( //���ϴ� �Լ��� ��������Ʈ�� ���ε�
		[&]()
		{
			UE_LOG(LogTemp,Log,TEXT("SUCCEEDED"));
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); //������ �Ǿ��ٰ� �˷��ִ� �Լ�
		}
	);

	AIPawn->SetAIAttackDelegate(OnAttackFinished);
	AIPawn->AttackByAI();
	return EBTNodeResult::InProgress; //���� �ִϸ��̼� ���� ���̱� ������ InProgress�� �����ϰ�, �ִϸ��̼� ���Ŀ� Succeded �� ��ȯ
}
