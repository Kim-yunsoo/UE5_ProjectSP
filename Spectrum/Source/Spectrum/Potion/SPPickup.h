// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SPCharacterPlayer.h"
#include "GameFramework/Actor.h"
#include "Interface/SPInteractionInterface.h"
#include "SPPickup.generated.h"

class UBoxComponent;
class USPItemBase;
class ASPPotionBase;

UCLASS()
class SPECTRUM_API ASPPickup : public AActor, public ISPInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPPickup();
	
	void InitializePickup(const TSubclassOf<USPItemBase>BaseClass, const int32 InQuantity); //데이터베이스에서 가지고 오기

	void InitializeDrop(USPItemBase* ItemToDrop,  const int32 InQuantity); //이미 존재하는 항목 참조해서 가지고 오기

	FORCEINLINE USPItemBase* GetItemData(){return ItemReference;};
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	TObjectPtr<UBoxComponent> Trigger;
	
	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	TObjectPtr<UStaticMeshComponent> PickupMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Pickup | Item Initialization")
	TObjectPtr<UDataTable> ItemDataTable;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Pickup | Item Initialization")
	FName DesiredItemID;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	TObjectPtr<USPItemBase> ItemReference; //pick up 가리키기

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	int32 ItemQuantity;

	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData;

	UPROPERTY()
	TObjectPtr<AActor> MyPlayerOwner;
	
protected:	
	// Called every frame

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	
	virtual void Interact(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget) override;
	virtual void Interact2(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget) override;
	void UpdateInteractableData();
	void TakePickup(ASPCharacterPlayer* Taker);
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override; //에디터있으면 실행
#endif

protected:
	UFUNCTION(Client, Unreliable)
	void ClientRPCUpdateWidget(ASPCharacterPlayer* Taker);

	UFUNCTION(Server, Unreliable)
	void ServerRPCInteract(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget);

	UFUNCTION()
	void OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
	
};
