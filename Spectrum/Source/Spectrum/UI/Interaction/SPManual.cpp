// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Interaction/SPManual.h"

#include "Character/SPCharacterPlayer.h"
#include "Components/BoxComponent.h"
#include "UI/SPHUDWidget.h"

// Sets default values
ASPManual::ASPManual()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	//SetRootComponent(Mesh);
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupTriggerComponent"));
}

// Called when the game starts or when spawned
void ASPManual::BeginPlay()
{
	Super::BeginPlay();
	InteractableData = InstanceInteractableDate;
	FVector ActorLocation = GetActorLocation();
	Trigger->SetRelativeLocation(ActorLocation);
	Trigger->SetRelativeScale3D(FVector(3,5,15));
	
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASPManual::OnTriggerEnter);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ASPManual::OnTriggerExit);
}

void ASPManual::OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASPCharacterPlayer* PlayerCharacter = Cast<ASPCharacterPlayer>(OtherActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->PerformInteractionCheck();
			UE_LOG(LogTemp,Warning, "ASPManual In!!!");
	}
}

void ASPManual::OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ASPCharacterPlayer* PlayerCharacter = Cast<ASPCharacterPlayer>(OtherActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->NoInteractableFound();
			UE_LOG(LogTemp,Warning, "ASPManual OUT!!!");
	}
}

// Called every frame
void ASPManual::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASPManual::BeginFocus()
{
	if(Mesh)
	{
		Mesh->SetRenderCustomDepth(true);
	}
}

void ASPManual::EndFocus()
{
	if(Mesh)
	{
		Mesh->SetRenderCustomDepth(false);
	}
}

void ASPManual::BeginInteract()
{
	ISPInteractionInterface::BeginInteract();
}

void ASPManual::EndInteract()
{
	ISPInteractionInterface::EndInteract();
}

void ASPManual::Interact(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget)
{
	ISPInteractionInterface::Interact(PlayerCharacter, HUDWidget);
}

void ASPManual::Interact2(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget)
{
	// if(bIsVisible)
	// {
	// 	bIsVisible = false;
	// 	HUDWidget->UpdateManualWidget(false);
	// }
	// else
	{
		HUDWidget->UpdateManualWidget(true);
		HUDWidget->ToggleMouse(true);
	}
}

