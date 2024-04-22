// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SPCharacterPlayer.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interface/SPGetInterface.h"
#include "Data/ItemDataStructs.h"
#include "SPPotionBase.generated.h"

UCLASS()
class SPECTRUM_API ASPPotionBase : public AActor, public ISPGetInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPPotionBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> MovementComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> PotionMesh;

	bool bHasExecutedOnce;
public:
	void Throw(const FVector& PotionDirection);

	void MoveTo();

	virtual  void GetPotion() override;

///////////////////////////////////////////////////////////////////////////////////////////

public:
	// UPROPERTY()
	// UInventoryComponent* OwningInventory;
	
	UPROPERTY(VisibleAnywhere, Category = "Item")
	int32 Quantity; //항목이 인벤토리로 이동할 때 수량
	
	UPROPERTY(VisibleAnywhere, Category = "Item")
	FName ID;
	
	UPROPERTY(VisibleAnywhere, Category = "Item")
	EItemType ItemType;
	
	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemTextData ItemTextData;
	
	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemNumericData ItemNumericData;
	
	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemAssetData ItemAssetData;

	UFUNCTION(Category = "Item")
	ASPPotionBase* CreatePotionCopy();

	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	UFUNCTION(Category = "Item")
	virtual void Use(ASPCharacterPlayer* Character);

protected: //항목 검색할 때 사용
	bool operator==(const FName& OtherID) const
	{
		return this->ID == OtherID;
	}
};
