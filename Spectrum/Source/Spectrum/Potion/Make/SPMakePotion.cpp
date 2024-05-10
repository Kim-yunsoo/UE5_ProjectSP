// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion/Make/SPMakePotion.h"
#include "SPMakingPotionWidget.h"
#include "Character/SPCharacterPlayer.h"
#include "Components/BoxComponent.h"
#include "UI/SPHUDWidget.h"

class ASPPlayerController;
// Sets default values
ASPMakePotion::ASPMakePotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	//Mesh->SetSimulatePhysics(true); //�ùķ��̼� true ����غ���
	//SetRootComponent(Mesh);
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupTriggerComponent"));
}
// Called when the game starts or when spawned
void ASPMakePotion::BeginPlay()
{
	Super::BeginPlay();
	InteractableData = InstanceInteractableDate;
	FVector ActorLocation = GetActorLocation();
	FRotator AcotrRotation = GetActorRotation();
	Trigger->SetRelativeLocation(ActorLocation + TriggerOffset);
	Trigger->SetRelativeRotation(AcotrRotation);
	Trigger->SetRelativeScale3D(FVector(2,7,8));
	
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASPMakePotion::OnTriggerEnter);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ASPMakePotion::OnTriggerExit);
}

// Called every frame
void ASPMakePotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ASPMakePotion::Interact2(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget)
{
	HUDWidget->UpdateMakingPotionWidget(true);
	HUDWidget->ToggleMouse(true);
	return true;
}

void ASPMakePotion::EndInteract()
{
	//ISPInteractionInterface::EndInteract();
	//bIsVisible = false;
	//HUDWidgetTest->UpdateMakingPotionWidget(false);

}

void ASPMakePotion::ClientRPCInteract_Implementation(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget)
{
	
}



void ASPMakePotion::OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASPCharacterPlayer* PlayerCharacter = Cast<ASPCharacterPlayer>(OtherActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->PerformInteractionCheck();
	}
}

void ASPMakePotion::OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	ASPCharacterPlayer* PlayerCharacter = Cast<ASPCharacterPlayer>(OtherActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->NoInteractableFound();
	}
}

