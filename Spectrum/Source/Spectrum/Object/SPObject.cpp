// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/SPObject.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ASPObject::ASPObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	SetActorEnableCollision(true);
	PrimaryActorTick.bCanEverTick = true;
	bHasBeenCalled = false; // 한번만 실행하기 위한 변수
	//MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	ObjectMesh->SetMobility(EComponentMobility::Movable);

}

// Called when the game starts or when spawned
void ASPObject::BeginPlay()
{
	Super::BeginPlay();
	RootComponent->SetMobility(EComponentMobility::Movable);
}

void ASPObject::OnExplosionHit(float Damage)
{
	if (false == bHasBeenCalled) 
	{

	}

}

// Called every frame
void ASPObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FLatentActionInfo LatentInfo;
	UKismetSystemLibrary::Delay(GetWorld(), 0.5, LatentInfo);

	if (ObjectLocation == GetActorLocation())
	{

	}
	else
	{
		ObjectLocation = GetActorLocation();
	}
}

