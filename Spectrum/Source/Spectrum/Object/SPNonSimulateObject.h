// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPObject.h"
#include "GameFramework/Actor.h"
#include "SPNonSimulateObject.generated.h"

UCLASS()
class SPECTRUM_API ASPNonSimulateObject : public ASPObject
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	ASPNonSimulateObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override; 
	
	FTimerHandle TimerHandle;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void StartPhysicsSleepTimer();
	void PhysicsSleepIfNeed();
};
