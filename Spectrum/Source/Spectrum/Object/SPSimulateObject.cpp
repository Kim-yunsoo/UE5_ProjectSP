// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/SPSimulateObject.h"

// Sets default values
ASPSimulateObject::ASPSimulateObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;
	ObjectMesh->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void ASPSimulateObject::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
// void ASPSimulateObject::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// }

