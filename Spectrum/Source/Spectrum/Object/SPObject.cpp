// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/SPObject.h"

// Sets default values
ASPObject::ASPObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	SetActorEnableCollision(true);
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASPObject::BeginPlay()
{
	Super::BeginPlay();
	RootComponent->SetMobility(EComponentMobility::Movable);
}

// Called every frame
void ASPObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FVector ActorWorldLocation = GetActorLocation();
	//UE_LOG(LogTemp, Log, TEXT("Actor's World Location: %s"), *ActorWorldLocation.ToString());
	//FRotator ActorWorldRotation = GetActorRotation();
	//UE_LOG(LogTemp, Log, TEXT("Actor's World Rotation: %s"), *ActorWorldRotation.ToString());
}

