// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/SPPotionSpawnInterface.h"
#include "SPPotionSpawner.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnSpawnerEmpty)


UCLASS()
class SPECTRUM_API ASPPotionSpawner : public AActor, public ISPPotionSpawnInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	FOnSpawnerEmpty OnSpawnerEmpty;
	ASPPotionSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION(Server,Unreliable)
	void ServerSpanwRPC();

	UFUNCTION()
	void SetItemSpawnEmpty();

	UPROPERTY()
	bool bHasPotion ;

public:
	FORCEINLINE bool GetHasPotion() const {return bHasPotion;};
	void SpawnPotion();
	virtual void SpawnEvent() override;
	float SpawnTimeRate ;




// public:	
// 	// Called every frame
// 	virtual void Tick(float DeltaTime) override;

};
