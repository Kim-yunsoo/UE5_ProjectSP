// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetMovementSpeed.generated.h"

enum class MovementSpeed : uint8;
/**
 * 
 */
UCLASS()
class SPECTRUM_API UBTTask_SetMovementSpeed : public UBTTaskNode
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditInstanceOnly,BlueprintReadWrite)
	MovementSpeed Speed;
	
};
