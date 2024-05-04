// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/SPInteractionInterface.h"
#include "SPCombination.generated.h"

class UBoxComponent;

UCLASS()
class SPECTRUM_API ASPCombination : public AActor, public ISPInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPCombination();

protected:
	UPROPERTY(EditAnywhere, Category = "Test Actor")
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditInstanceOnly, Category = "Test Actor")
	FInteractableData InstanceInteractableDate;
	// Called when the game starts or when spawned
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Trigger;
	
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
	virtual void Interact(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget) override;
	virtual void Interact2(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget) override;
};
