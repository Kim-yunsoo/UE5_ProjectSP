// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ItemDataStructs.h"
#include "SPInventoryComponent.generated.h"

UENUM()
enum class EPotion :uint8
{
	Green = 0,
	Orange ,
	Purple,
	Black ,
	Special 
};

UENUM()
enum class EPotionType : uint8
{
	BluePotion ,
	RedPotion ,
	YellowPotion,
	GreenPotion ,
	PurplePotion,
	OrangePotion
};

class USPItemBase;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdated, const TArray<USPItemBase*>);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryMiniUpdated, const TArray<USPItemBase*>);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPECTRUM_API USPInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnInventoryUpdated OnInventoryUpdated;
	FOnInventoryUpdated OnInventoryMiniUpdated;
	// Sets default values for this component's properties
	USPInventoryComponent();
	UFUNCTION(Category = "Inventory")
	void HandleAddItem(USPItemBase* InputItem, int InitialRequestedAddmount);

	UFUNCTION()
	USPItemBase* InitializeInventory(const TSubclassOf<USPItemBase> BaseClass, FName DesiredItemID);

	UFUNCTION(Category = "Inventory")	
	USPItemBase* FindMiniItem(FName ID);

	UFUNCTION(Category = "Inventory")	
	USPItemBase* FindPotionItem(FName ID);
	
	UFUNCTION(Category = "Inventory")	
	int CountPotion(int num);
	
	UFUNCTION(Category = "Inventory")	
	void RemoveInventorMakeContents(USPItemBase* ItemToRemove);
	UFUNCTION(Category = "Inventory")
	USPItemBase* FindMatchingItem(int Num);

	UFUNCTION(Category = "Inventory")
	USPItemBase* FindMatchingMiniItem(int Num);
	UFUNCTION(Category = "Inventory")
	USPItemBase* FindItem(USPItemBase* ItemIn, EItemType Potion) const;
	UFUNCTION(Category = "Inventory")
	void RemoveSingleinstanceOfItem(USPItemBase* ItemToRemove, EItemType Potion); //인벤토리 배열에서 삭제
	UFUNCTION(Category = "Inventory")
	int32 RemoveAmountOfItem(USPItemBase* ItemIn, int32 DesiredAmountToRemove); //인벤토리는 있고 부분적으로 삭제
	
	UPROPERTY(EditInstanceOnly, Category = "Inventory | Item Initialization")
	TObjectPtr<UDataTable> ItemDataTable;
	
	UFUNCTION(Category = "Inventory")
	USPItemBase* MakingPotion();
	

	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<USPItemBase*> GetInventoryContents() const{return InventoryContents;};
	
	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<USPItemBase*> GetInventorMiniContents() {return InventoryMiniContents;};
	
	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<USPItemBase*> GetInventorMakeContents() {return InventoryMakeContents;};

	UFUNCTION(Category = "Inventory")
	FORCEINLINE void AddInventorMakeContents(USPItemBase* Item) {InventoryMakeContents.Add(Item);};
	FORCEINLINE void ClearMakeArray() {InventoryMakeContents.Empty();};


	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	float InventoryTotalWeight;
	UPROPERTY(EditInstanceOnly, Category = "Inventory")
	int32 InventorySlotsCapacity;
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	float inventoryWeightCapacity;
	
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<USPItemBase*> InventoryMiniContents;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<USPItemBase*> InventoryMakeContents;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<USPItemBase*> InventoryContents;
	
	int HandleStackableItems(USPItemBase* ItemIn, int32 RequestedAddAmount,const EItemType& InItemType);
	//int HandleStackableItemsMini(USPItemBase* ItemIn, int32 RequestedAddAmount);

public:
	int IsPotion(FName  ID);
	int IsMiniPotion(FName  ID);
//RPC
protected:
	UFUNCTION(Client, Unreliable)
	void ClientRPCUpdatePotion(const int& num, const int&ServerCount , const EItemType& InItemType);

	UFUNCTION(Client, Unreliable)
	void ClientRPCUpdateMiniPotion(const int& num, const int&ServerCount);
	
	TMap<FName, int32> ItemCountMap;

	int32 SinglePotion;
	int32 DoublePotion;

	FName GetPotionName(const EPotionType& InPotionType );

	USPItemBase* CreateItemFromDataTable(const FName& InDesiredItemID);

};
