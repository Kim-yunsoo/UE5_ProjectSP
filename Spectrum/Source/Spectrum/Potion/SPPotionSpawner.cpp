// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion/SPPotionSpawner.h"
#include "SPPickup.h"

// Sets default values
ASPPotionSpawner::ASPPotionSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;
	SpawnTimeRate = 5.f;
	bReplicates = true;
	bHasPotion = false;
}

// Called when the game starts or when spawned
void ASPPotionSpawner::BeginPlay()
{
	Super::BeginPlay();
	// if (HasAuthority())
	// {
	// 	//SpawnPotion();
	// }
}

void ASPPotionSpawner::SpawnPotion() //신호가 오면 스폰하는 역할을 한다. 
{
	ServerSpanwRPC();
}

void ASPPotionSpawner::SpawnEvent()
{
	// FTimerHandle TimerHandle;
	// GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]
	// {
	// 	ServerSpanwRPC();
	// }), SpawnTimeRate, false);
}

void ASPPotionSpawner::SetItemSpawnEmpty()
{
	bHasPotion =false; //없음 표시
	OnSpawnerEmpty.Broadcast();
}


void ASPPotionSpawner::ServerSpanwRPC_Implementation()
{
	bHasPotion = true;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
	ASPPickup* PotionObject = GetWorld()->SpawnActor<ASPPickup>(ASPPickup::StaticClass(), GetActorLocation(),
	                                                            GetActorRotation(), SpawnParams);
	if (PotionObject)
	{
		PotionObject->SetOwner(this);
	//바인드
		PotionObject->OnItemPickedUp.AddUObject(this, &ASPPotionSpawner::SetItemSpawnEmpty); //델리게이트 걸어주기 
	}
}


// // Called every frame
// void ASPPotionSpawner::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// }
