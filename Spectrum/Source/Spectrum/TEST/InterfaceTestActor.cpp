// Fill out your copyright notice in the Description page of Project Settings.


#include "TEST/InterfaceTestActor.h"
#include "Character/SPCharacterPlayer.h"

#include "NavigationSystemTypes.h"

// Sets default values
AInterfaceTestActor::AInterfaceTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void AInterfaceTestActor::BeginPlay()
{
	Super::BeginPlay();

	InteractableData = InstanceInteractableDate;
}

// Called every frame
void AInterfaceTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInterfaceTestActor::BeginFocus()
{
	if(Mesh)
	{
		Mesh->SetRenderCustomDepth(true);
	}
}

void AInterfaceTestActor::EndFocus()
{
	if(Mesh)
	{
		Mesh->SetRenderCustomDepth(false);
	}
}

void AInterfaceTestActor::BeginInteract()
{
	UE_LOG(LogTemp, Log, TEXT("BeginInteract"));
}

void AInterfaceTestActor::EndInteract()
{
	UE_LOG(LogTemp, Log, TEXT("EndInteract"));

}

void AInterfaceTestActor::Interact(ASPCharacterPlayer* PlayerCharacter)
{
	UE_LOG(LogTemp, Log, TEXT("Interact"));

}

