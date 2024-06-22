// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "SPPotionSpawnerManager.generated.h"
class ASPPotionSpawner;

UCLASS()
class SPECTRUM_API USPPotionSpawnerManager : public UObject
{
	GENERATED_BODY()

	USPPotionSpawnerManager();
public:
	void Initialize(UWorld* InWorld); //Max 값 만큼 채워 넣는다.
	
protected:
	int32 MaxSpawnCount ;
	TArray<ASPPotionSpawner*> PotionSpawners;

	void CollectAllPotionSpawners(UWorld* InWorld); //스포너 모두 수집한다.

	UFUNCTION()
	void NotifyEmptySpawners(); 
	
	
};
