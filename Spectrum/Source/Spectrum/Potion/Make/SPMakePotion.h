// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/SPInteractionInterface.h"
#include "SPMakePotion.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnWidgetDelegate, bool /*aim*/)

UCLASS()
class SPECTRUM_API ASPMakePotion : public AActor,  public ISPInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPMakePotion();
	FOnWidgetDelegate OnWidgetDelegate;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void Interact(ASPCharacterPlayer* PlayerCharacter) override;

	UPROPERTY()
	TObjectPtr<class USPMakingPotionWidget> MakingPotionWidget;
	
	UPROPERTY()
	uint8 bIsVisible : 1;

	UPROPERTY(VisibleAnywhere, Category = "MakePotion | Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditInstanceOnly, Category = "Test Actor")
	FInteractableData InstanceInteractableDate;
};
