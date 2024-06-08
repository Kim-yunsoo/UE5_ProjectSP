// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_StopAttackTargetIsDead.h"

#include "SPAI.h"
#include "SPAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/SPCharacterNonPlayer.h"

UBTService_StopAttackTargetIsDead::UBTService_StopAttackTargetIsDead()
{
	Interval = 1.0f;
	RandomDeviation=0.0f;
}

void UBTService_StopAttackTargetIsDead::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	UObject* TargetActor = OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET);

	ASPCharacterNonPlayer* AIPlayer = Cast<ASPCharacterNonPlayer>(TargetActor);
	AAIController* AIController = OwnerComp.GetAIOwner();
	ASPAIController* MyController = Cast<ASPAIController>(AIController);
	if(AIPlayer)
	{
		if(AIPlayer->IsDead())
		{
			if(MyController)
			{
				MyController->SetStatePassvie();
			}
		}
	}
}
