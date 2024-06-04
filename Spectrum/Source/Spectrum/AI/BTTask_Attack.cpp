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

	FAICharacterAttackFinished OnAttackFinished; //델리게이트 선언
	OnAttackFinished.BindLambda( //원하는 함수를 델리게이트에 바인드
		[&]()
		{
			UE_LOG(LogTemp,Log,TEXT("SUCCEEDED"));
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); //마무리 되었다고 알려주는 함수
		}
	);

	AIPawn->SetAIAttackDelegate(OnAttackFinished);
	AIPawn->AttackByAI();
	return EBTNodeResult::InProgress; //공격 애니메이션 진행 중이기 때문에 InProgress로 설정하고, 애니메이션 이후에 Succeded 값 반환
}
