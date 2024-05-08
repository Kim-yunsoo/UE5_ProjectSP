// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemDataStructs.h"
#include "SPItemBase.generated.h"

class USPInventoryComponent;
class UInventoryComponent;
class ASPCharacterPlayer;
/**
 * 
 */
UCLASS()
class SPECTRUM_API USPItemBase : public UObject
{
	GENERATED_BODY()
public:

	USPItemBase();
	bool bIsCopy;
	bool bIsPickup;

	void ResetItemFlags();
		
	UPROPERTY()
	TObjectPtr<USPInventoryComponent> OwningInventory;
	
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
	USPItemBase* CreatePotionCopy() const;

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
