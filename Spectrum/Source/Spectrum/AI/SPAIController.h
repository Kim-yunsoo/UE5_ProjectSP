// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISense.h"
#include "SPAIController.generated.h"

enum class EAISense : uint8;
enum class AIState : uint8;

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
	UPROPERTY(Blueprintable, EditAnywhere,BlueprintReadWrite )
	TObjectPtr<AActor> AttackTarget;
	
	UFUNCTION(BlueprintCallable)
	void SetStatePassvie();
	UFUNCTION(BlueprintCallable)
	void SetStateAttacking(AActor* Target);
	//UFUNCTION(BlueprintCallable)
	void SetStateAsDead();
	UFUNCTION()
	void HandleSightSense (AActor* Actor, FAIStimulus Stimulus);

	void HandleSensedSight(AActor* Actor);
	void HandleSensedSound(const FVector Location);
	void HandleSensedDamage(AActor* Actor);
	void SetStateAsInvestigating(const FVector Location);
	void SetStateAsFrozen();
	
	AIState GetCurrentState();
	
	EAISense GetSenseEnum(const FName& SenseType);
};

