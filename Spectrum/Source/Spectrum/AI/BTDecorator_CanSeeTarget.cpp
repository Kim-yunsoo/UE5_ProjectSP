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

	FVector Start = ControllingPawn->GetActorLocation(); // AI ��ġ ���� 
	FVector End =TargetActor->GetActorLocation(); //�������� Ÿ���� ��ġ ����

	// ���� Ʈ���̽��� ���� ��Ʈ ��� ����ü
	FHitResult OutHit;

	// �浹 ä���� ���� (���⼭�� ECC_Visibility�� ���)
	ECollisionChannel TraceChannel = ECC_Visibility;

	// ���� Ʈ���̽��� ����� �Ķ���� ����
	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = false;
	TraceParams.AddIgnoredActor(ControllingPawn); //��� ���ʹ� ���� 
	
	//TraceParams.bReturnPhysicalMaterial = false;

	// ����׿� ���� �׸���
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

	// ���� Ʈ���̽� ����
	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, TraceChannel, TraceParams);

	return !bHit; //�浹�� ���� ��� true�� ��ȯ�Ѵ�. 
}
