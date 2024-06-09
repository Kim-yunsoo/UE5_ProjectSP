// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsHealthBelow.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API UBTDecorator_IsHealthBelow : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsHealthBelow();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditInstanceOnly,BlueprintReadWrite)
	float HealthThreshold; //0.3À¸·Î 
	
};
