// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "Component/SPComponentBase.h"

// Sets default values for this component's properties
USPComponentBase::USPComponentBase()
{
	bHasExecutedOnce = false;
}


// Called when the game starts
void USPComponentBase::BeginPlay()
{
	Super::BeginPlay();
}

void USPComponentBase::Explode()
{
	//Spawn Emitter at Location�� ���� ����
	FVector ActorLocation = GetOwner()->GetActorLocation(); //������ ������ ��ġ 
	FVector Location = ActorLocation;
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
	FVector Scale{ 1.0f,1.0f,1.0f };
	FTransform SpawnTransform{ Rotation,Location,Scale };
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Effect, SpawnTransform, true, EPSCPoolMethod::None, true);
}
