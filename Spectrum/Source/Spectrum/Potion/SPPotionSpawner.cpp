// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion/SPPotionSpawner.h"

#include "SpectrumLog.h"
#include "SPPickup.h"

// Sets default values
ASPPotionSpawner::ASPPotionSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;
	SpawnTimeRate=30.f;
	SetReplicates(true);
}

// Called when the game starts or when spawned
void ASPPotionSpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnPotion();
}

void ASPPotionSpawner::SpawnPotion()
{
	ServerSpanwRPC();
}

void ASPPotionSpawner::SpawnEvent()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		ServerSpanwRPC();
	}), SpawnTimeRate, false);
}

void ASPPotionSpawner::ServerSpanwRPC_Implementation()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
	ASPPickup* PotionObject = GetWorld()->SpawnActor<ASPPickup>(ASPPickup::StaticClass(), GetActorLocation(),
	                                                            GetActorRotation(), SpawnParams);
	// Register()
	if (PotionObject)
	{
		PotionObject->SetOwner(this);
	}
}


// // Called every frame
// void ASPPotionSpawner::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// }
