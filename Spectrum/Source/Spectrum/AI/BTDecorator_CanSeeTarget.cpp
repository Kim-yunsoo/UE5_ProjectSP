// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_CanSeeTarget.h"

#include "AIController.h"
#include "SPAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

class ISPCharacterAIInterface;

bool UBTDecorator_CanSeeTarget::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult =  Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}
	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET);
	AActor* TargetActor = Cast<AActor>(TargetObject);

	FVector Start = ControllingPawn->GetActorLocation(); // AI 위치 지점 
	FVector End =TargetActor->GetActorLocation(); //끝지점은 타겟의 위치 지점

	// 라인 트레이스를 위한 히트 결과 구조체
	FHitResult OutHit;

	// 충돌 채널을 설정 (여기서는 ECC_Visibility를 사용)
	ECollisionChannel TraceChannel = ECC_Visibility;

	// 라인 트레이스에 사용할 파라미터 설정
	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = false;
	TraceParams.AddIgnoredActor(ControllingPawn); //쏘는 액터는 무시 
	
	//TraceParams.bReturnPhysicalMaterial = false;

	// 디버그용 라인 그리기
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

	// 라인 트레이스 실행
	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, TraceChannel, TraceParams);

	return !bHit; //충돌이 없는 경우 true를 반환한다. 
}
