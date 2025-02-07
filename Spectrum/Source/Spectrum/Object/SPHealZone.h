// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPHealZone.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpUpDelegate);

UCLASS()
class SPECTRUM_API ASPHealZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPHealZone();
	FOnHpUpDelegate OnHpUpDelegate;
	FTimerHandle TimerHandle;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	void SpawnHealSphere();

	UPROPERTY()
	TObjectPtr<UParticleSystemComponent> HealEffect;

};
