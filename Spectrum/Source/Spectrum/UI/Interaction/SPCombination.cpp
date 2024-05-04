// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Interaction/SPCombination.h"

#include "Character/SPCharacterPlayer.h"
#include "Components/BoxComponent.h"

// Sets default values
ASPCombination::ASPCombination()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	//SetRootComponent(Mesh);
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupTriggerComponent"));
}

// Called when the game starts or when spawned
void ASPCombination::BeginPlay()
{
	Super::BeginPlay();
	InteractableData = InstanceInteractableDate;
	FVector ActorLocation = GetActorLocation();
	Trigger->SetRelativeLocation(ActorLocation);
	Trigger->SetRelativeScale3D(FVector(6,5,20));
	
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASPCombination::OnTriggerEnter);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ASPCombination::OnTriggerExit);
}

void ASPCombination::OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASPCharacterPlayer* PlayerCharacter = Cast<ASPCharacterPlayer>(OtherActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->PerformInteractionCheck();
	}
}

void ASPCombination::OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ASPCharacterPlayer* PlayerCharacter = Cast<ASPCharacterPlayer>(OtherActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->NoInteractableFound();
	}
}

// Called every frame
void ASPCombination::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASPCombination::BeginFocus()
{
	if(Mesh)
	{
		Mesh->SetRenderCustomDepth(true);
	}
}

void ASPCombination::EndFocus()
{
	if(Mesh)
	{
		Mesh->SetRenderCustomDepth(false);
	}
}

void ASPCombination::BeginInteract()
{
	ISPInteractionInterface::BeginInteract();
}

void ASPCombination::EndInteract()
{
	ISPInteractionInterface::EndInteract();
}

void ASPCombination::Interact(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget)
{
	ISPInteractionInterface::Interact(PlayerCharacter, HUDWidget);
}

void ASPCombination::Interact2(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget)
{
	ISPInteractionInterface::Interact2(PlayerCharacter, HUDWidget);
}

