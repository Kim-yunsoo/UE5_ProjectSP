// Fill out your copyright notice in the Description page of Project Settings.
#include "AI/SPAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "SPAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/SPCharacterPlayer.h"
#include "Enums/SPAISense.h"
#include "Enums/SPAIState.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

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
		SightConfig->PeripheralVisionAngleDegrees = 60.0f; //�þ� ��
		SightConfig->SetMaxAge(5.0f); // �þ� �ݰ濡�� ������� �� �󸶳� ���� ����ϴ���?
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		AIPerception->ConfigureSense(*SightConfig);

		UAISenseConfig_Hearing* SoundConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("SoundConfig"));
		SoundConfig->HearingRange = 500.0f;
		SoundConfig->SetMaxAge(3.0f);
		SoundConfig->DetectionByAffiliation.bDetectEnemies = true;
		SoundConfig->DetectionByAffiliation.bDetectNeutrals = true;
		SoundConfig->DetectionByAffiliation.bDetectFriendlies = true;
		AIPerception->ConfigureSense(*SoundConfig);


		UAISenseConfig_Damage* DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
		DamageConfig->SetMaxAge(5.0f);
		AIPerception->ConfigureSense(*DamageConfig);


		AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());
		//GetSenseImplementation�� UAISense_Sight�Լ��� ��ȯ�Ѵ�. 
		//�켼 ���� ����
	}

	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ASPAIController::HandleSightSense);
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

void ASPAIController::SetStateAttacking(AActor* Target)
{
	Blackboard->SetValueAsObject(BBKEY_TARGET, Target);
	Blackboard->SetValueAsEnum(BBKEY_State, static_cast<uint8>(AIState::Attacking));
}

void ASPAIController::HandleSightSense(AActor* Actor, FAIStimulus Stimulus)
{
	EAISense SenseType = GetSenseEnum(Stimulus.Type.Name);

	if (SenseType == EAISense::Hearing)
	{
		UE_LOG(LogTemp, Log, TEXT("Hearing"));
	}
	if (SenseType == EAISense::Sight)
	{
		UE_LOG(LogTemp, Log, TEXT("Sight"));
		HandleSensedSight(Actor);
	}
	if (SenseType == EAISense::Damage)
	{
		UE_LOG(LogTemp, Log, TEXT("Damage"));
	}
}

void ASPAIController::HandleSensedSight(AActor* Actor)
{
	//GetCurrentState();
	if (GetCurrentState() == AIState::Passive || GetCurrentState() == AIState::Investigating)
	{
		if (Cast<ACharacter>(Actor)) //���÷��̾ ������ 
		{
			SetStateAttacking(Actor);
		}
	}
}

AIState ASPAIController::GetCurrentState()
{
	uint8 EnumValue = Blackboard->GetValueAsEnum(BBKEY_State);
	return static_cast<AIState>(EnumValue);
}

EAISense ASPAIController::GetSenseEnum(const FName& SenseType)
{
	if (SenseType == "Default__AISense_Hearing")
	{
		return EAISense::Hearing;
	}
	else if (SenseType == "Default__AISense_Sight")
	{
		return EAISense::Sight;
	}
	else if (SenseType == "Default__AISense_Damage")
	{
		return EAISense::Damage;
	}

	return EAISense::None;
}
