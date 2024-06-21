// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/SPCharacterHUDInterface.h"
#include "Interface/SPInteractionInterface.h"
#include "SPMakePotion.generated.h"


class UBoxComponent;

UCLASS()
class SPECTRUM_API ASPMakePotion : public AActor,  public ISPInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPMakePotion();
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Trigger;
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual bool Interact(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget) override;
	virtual void EndInteract() override;
protected:
	UPROPERTY(EditInstanceOnly, Category = "Test Actor")
	FInteractableData InstanceInteractableDate;
	
	UPROPERTY()
	TObjectPtr<class USPMakingPotionWidget> MakingPotionWidget;
	
	UPROPERTY()
	uint8 bIsVisible : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TriggerOffset;
	
	UPROPERTY(VisibleAnywhere, Category = "MakePotion | Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UFUNCTION(Client, Unreliable)
	void ClientRPCInteract(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget);

	UFUNCTION()
	void OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
