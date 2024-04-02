// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/SPObject.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

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
	ObjectLocation = GetActorLocation();
}

void ASPObject::OnExplosionHit(float Damage)
{
	if (false == bHasBeenCalled)
	{
		ObjectMesh->SetHiddenInGame(true);
		ObjectMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		this->SetLifeSpan(1.0f);
		bHasBeenCalled = true;
	}
}

// Called every frame
void ASPObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	static float DelayTime = 1.0;
	DelayTime -= DeltaTime;
	if (DelayTime > 0.0f)
	{
		return;
	}
	DelayTime = 1.0;
	bool Equal = UKismetMathLibrary::EqualEqual_VectorVector(ObjectLocation, GetActorLocation(), 0.0);
	if (Equal)
	{
		ObjectMesh->SetSimulatePhysics(false);
		ObjectLocation = GetActorLocation();
	}
	else
	{
		ObjectLocation = GetActorLocation();
	}

}

