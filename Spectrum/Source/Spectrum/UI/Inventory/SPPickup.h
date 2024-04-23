// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SPCharacterPlayer.h"
#include "GameFramework/Actor.h"
#include "Interface/SPInteractionInterface.h"
#include "SPPickup.generated.h"

class ASPPotionBase;

UCLASS()
class SPECTRUM_API ASPPickup : public AActor, public ISPInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPPickup();
	
	void InitializePickup(const TSubclassOf<ASPPotionBase>CaseClass, const int32 InQuantity); //데이터베이스에서 가지고 오기

	void InitializeDrop(ASPPotionBase* ItemToDrop,  const int32 InQuantity); //이미 존재하는 항목 참조해서 가지고 오기

	FORCEINLINE ASPPotionBase* GetItemData(){return ItemReference;};
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	TObjectPtr<UStaticMeshComponent> PickupMesh;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	TObjectPtr<UDataTable> ItemDataTable;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Database")
	FName DesiredItemID;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	TObjectPtr<ASPPotionBase> ItemReference; //pick up 가리키기

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Reference")
	int32 ItemQuantity;

	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData;
	
public:	
	// Called every frame

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	
	virtual void Interact(ASPCharacterPlayer* PlayerCharacter) override;
	void TakePickup(const ASPCharacterPlayer* Taker);
};
