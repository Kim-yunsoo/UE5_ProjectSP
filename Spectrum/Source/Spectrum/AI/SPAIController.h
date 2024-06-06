// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SPAIController.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API ASPAIController : public AAIController
{
	GENERATED_BODY()
public:
	ASPAIController();

	void RunAI();
	void StopAI();
protected:
	//어떤 컨트롤러가 폰에 빙의할때 발생되는 이벤트 함수 
	virtual void OnPossess(APawn* InPawn) override;
	
private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;
	
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

	UPROPERTY()
	TObjectPtr<UAIPerceptionComponent> AIPerception;

	

public:
	UFUNCTION(BlueprintCallable)
	void SetStatePassvie();

	UFUNCTION(BlueprintCallable)
	void SetStateAttacking(APawn* TargetPawn);

	
};

