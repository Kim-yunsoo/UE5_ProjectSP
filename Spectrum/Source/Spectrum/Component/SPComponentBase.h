// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SPComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPECTRUM_API USPComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USPComponentBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	bool bHasExecutedOnce;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystem> Effect;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*>ActorArray ;
protected:
	virtual void Explode();

	UPROPERTY()
	TObjectPtr<UParticleSystem> EmitterHit;
};
