// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/SPPotionSpawnInterface.h"
#include "SPPotionSpawner.generated.h"

UCLASS()
class SPECTRUM_API ASPPotionSpawner : public AActor, public ISPPotionSpawnInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPPotionSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnPotion();


	UFUNCTION(Server,Unreliable)
	void ServerSpanwRPC();

public:
	virtual void SpawnEvent() override;

	float SpawnTimeRate ;
// public:	
// 	// Called every frame
// 	virtual void Tick(float DeltaTime) override;

};
