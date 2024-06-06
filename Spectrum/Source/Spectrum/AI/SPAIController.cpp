// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SPAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "SPAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enums/SPAIState.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"

ASPAIController::ASPAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(
		TEXT("/Script/AIModule.BlackboardData'/Game/AI/BB_SP_Character.BB_SP_Character'"));
	if (nullptr != BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(
		TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BT_SP_Character.BT_SP_Character'"));
	if (nullptr != BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
	if (AIPerception)
	{
		UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
		SightConfig->SightRadius = 800.0f; //�þ߹ݰ�
		SightConfig->LoseSightRadius = 1200.0f;
		SightConfig->PeripheralVisionAngleDegrees=60.0f; //�þ� ��
		SightConfig->SetMaxAge(5.0f); // �þ� �ݰ濡�� ������� �� �󸶳� ���� ����ϴ���?
		SightConfig->DetectionByAffiliation.bDetectEnemies =true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals =true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies =true;
		

		
		AIPerception->ConfigureSense(*SightConfig);
		//���� �߰�
		AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());
		//GetSenseImplementation�� UAISense_Sight�Լ��� ��ȯ�Ѵ�. 
		//�켼 ���� ����
	}
}

void ASPAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr)) // ������ �⵿��Ű�� �Լ� 
	{
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
		SetStatePassvie();

		//SetStateAttacking();
		//Blackboard->SetValueAsEnum(BBKEY_State, static_cast<uint8>(AIState::Attacking));
	}
}

void ASPAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void ASPAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI(); //AI ����
}

void ASPAIController::SetStatePassvie()
{
	Blackboard->SetValueAsEnum(BBKEY_State, static_cast<uint8>(AIState::Passive));
}

void ASPAIController::SetStateAttacking(APawn* TargetPawn)
{
	Blackboard->SetValueAsObject(BBKEY_TARGET, TargetPawn);
	Blackboard->SetValueAsEnum(BBKEY_State, static_cast<uint8>(AIState::Attacking));
}
