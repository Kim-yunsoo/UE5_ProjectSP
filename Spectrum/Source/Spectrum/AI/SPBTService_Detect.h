// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SPBTService_Detect.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API USPBTService_Detect : public UBTService
{
	GENERATED_BODY()

public:
	USPBTService_Detect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
