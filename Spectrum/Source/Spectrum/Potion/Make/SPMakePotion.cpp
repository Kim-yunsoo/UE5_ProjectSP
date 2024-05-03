// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion/Make/SPMakePotion.h"

#include "SpectrumLog.h"
#include "SPMakingPotionWidget.h"
#include "UI/SPHUDWidget.h"

class ASPPlayerController;
// Sets default values
ASPMakePotion::ASPMakePotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetSimulatePhysics(true); //시뮬레이션 true 고민해보기
	SetRootComponent(Mesh);
}
// Called when the game starts or when spawned
void ASPMakePotion::BeginPlay()
{
	Super::BeginPlay();
	InteractableData = InstanceInteractableDate;
}

// Called every frame
void ASPMakePotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASPMakePotion::Interact2(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget)
{
	HUDWidgetTest = HUDWidget;
	if(bIsVisible)
	{
		bIsVisible = false;
		HUDWidget->UpdateMakingPotionWidget(false);
		HUDWidget->ToggleMenu();
	}
	else
	{
		bIsVisible = true;
		UE_LOG(LogTemp, Warning, TEXT("Interact"));
		SP_LOG(LogSPNetwork, Log, TEXT("%s"), TEXT("Interact"));
		HUDWidget->UpdateMakingPotionWidget(true);
		HUDWidget->ToggleMenu();
	}
	
	//ClientRPCInteract(PlayerCharacter, HUDWidget);
}

void ASPMakePotion::EndInteract()
{
	//ISPInteractionInterface::EndInteract();
	//bIsVisible = false;
	//HUDWidgetTest->UpdateMakingPotionWidget(false);

	//Todo 다 끝나면 위젯 나가야함
}

void ASPMakePotion::ClientRPCInteract_Implementation(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget)
{
	
}



