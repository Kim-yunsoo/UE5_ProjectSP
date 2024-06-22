// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion/SPPotionSpawnerManager.h"
#include "SPPotionSpawner.h"
#include "Kismet/GameplayStatics.h"

USPPotionSpawnerManager::USPPotionSpawnerManager()
{
	MaxSpawnCount=25; 
}

void USPPotionSpawnerManager::CollectAllPotionSpawners(UWorld* InWorld)
{
	if(!InWorld)
	{
		UE_LOG(LogTemp,Warning, TEXT("CollectAllPotionSpawners: InWorld is nullptr"));
		return ;
	}
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(InWorld, ASPPotionSpawner::StaticClass(),FoundActors);

	PotionSpawners.Empty(); //����
	
	for(AActor* Actor : FoundActors)
	{
		ASPPotionSpawner* Spawner = Cast<ASPPotionSpawner>(Actor);
		if(Spawner)
		{
			PotionSpawners.Add(Spawner);
		}
	}
}

void USPPotionSpawnerManager::NotifyEmptySpawners() //�������� ����� ��ȣ�� �´�. 
{
	TArray<int32> EmptyArray; //�� ���� ������ ���� 
	for(int i=0; i<PotionSpawners.Num(); ++i)
	{
		if(PotionSpawners[i]->GetHasPotion() == false)
		{
			EmptyArray.Add(i); 
		}
	}

	
	EmptyArray.Sort([this](const int32 Lhs ,const int32 Rhs) //���� 
	{
		return FMath::FRand()<0.5f;
	});

	int32 RandomIndex = FMath::RandRange(0, EmptyArray.Num() - 1);
	PotionSpawners[EmptyArray[RandomIndex]]-> SpawnPotion();
}

void USPPotionSpawnerManager::Initialize(UWorld* InWorld)
{
	CollectAllPotionSpawners(InWorld);

	// ������ ��ü �ϳ��ϳ� ���ε� �ɱ�
	for(int i=0; i<PotionSpawners.Num(); ++i)
	{
		PotionSpawners[i]->OnSpawnerEmpty.AddUObject(this, &USPPotionSpawnerManager::NotifyEmptySpawners);
	}
	
	TArray<int32> AllIndex;
	AllIndex.SetNum(PotionSpawners.Num());

	for(int i=0; i<AllIndex.Num(); ++i)
	{
		AllIndex[i] = i;
	}

	AllIndex.Sort([this](const int32 Lhs ,const int32 Rhs)
	{
		return FMath::FRand()<0.5f;
	});

	for(int i=0; i<MaxSpawnCount ; ++i)
	{
		PotionSpawners[AllIndex[i]]->SpawnPotion(); //���������� 20�� 
	}
}
