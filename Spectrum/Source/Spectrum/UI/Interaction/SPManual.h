// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/SPInteractionInterface.h"
#include "SPManual.generated.h"

class UBoxComponent;

UCLASS()
class SPECTRUM_API ASPManual : public AActor, public ISPInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPManual();

protected:
	UPROPERTY(EditAnywhere, Category = "Actor")
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditInstanceOnly, Category = "Actor")
	FInteractableData InstanceInteractableDate;
	// Called when the game starts or when spawned
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Trigger;



	UPROPERTY()
	uint8 bIsVisible : 1;
	
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
	virtual bool Interact(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget) override;
};
