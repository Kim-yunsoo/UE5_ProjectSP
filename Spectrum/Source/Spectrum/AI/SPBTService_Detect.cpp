// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SPBTService_Detect.h"
#include "SPAI.h"
#include "AIController.h"
#include "Interface/SPCharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
//#include "Physics/SPCollision.h"
#include "DrawDebugHelpers.h"
#include "Physics/SPCollision.h"

USPBTService_Detect::USPBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void USPBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) //Detect 사용시 틱 이벤트 발동
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return;
	}

	ISPCharacterAIInterface* AIPawn = Cast<ISPCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return;
	}

	float DetectRadius = AIPawn->GetAIDetectRange();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);
	
	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController()) //플레이어 캐릭터인 경우만 감지했다고 한다. 
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn); //타겟값 저장 
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
	
				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);
				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr); //플레이어를 찾지 못한 경우 
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f); //붉은 색
}
