// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_StopAttackTargetIsDead.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API UBTService_StopAttackTargetIsDead : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_StopAttackTargetIsDead();

	UPROPERTY(EditInstanceOnly,BlueprintReadWrite)
	FBlackboardKeySelector AttackTargetKey;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
