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
	void Initialize(UWorld* InWorld); //Max �� ��ŭ ä�� �ִ´�.
	
protected:
	int32 MaxSpawnCount ;
	TArray<ASPPotionSpawner*> PotionSpawners;

	void CollectAllPotionSpawners(UWorld* InWorld); //������ ��� �����Ѵ�.

	UFUNCTION()
	void NotifyEmptySpawners(); 
	
	
};
