// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion/Make/SPMakePotion.h"

#include "SPMakingPotionWidget.h"
#include "Blueprint/UserWidget.h"

class ASPPlayerController;
// Sets default values
ASPMakePotion::ASPMakePotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	Mesh->SetSimulatePhysics(true); //½Ã¹Ä·¹ÀÌ¼Ç true °í¹ÎÇØº¸±â
	SetRootComponent(Mesh);
}
// Called when the game starts or when spawned
void ASPMakePotion::BeginPlay()
{
	Super::BeginPlay();
	InteractableData = InstanceInteractableDate;
	//MakingPotionWidget = Cast<USPMakingPotionWidget>(GetWidgetFromName(TEXT("WBP_SPMakingPotionWidget")));
	MakingPotionWidget = CreateWidget<USPMakingPotionWidget>(this, UserWidgetClass);
}

// Called every frame
void ASPMakePotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASPMakePotion::Interact(ASPCharacterPlayer* PlayerCharacter)
{
	if(bIsVisible)
	{
		bIsVisible = false;
		UE_LOG(LogTemp, Warning, TEXT("TESTTESTETST"));
		OnWidgetDelegate.Broadcast(bIsVisible);
	}
	else
	{
		bIsVisible = true;
		OnWidgetDelegate.Broadcast(bIsVisible);
	}
}


