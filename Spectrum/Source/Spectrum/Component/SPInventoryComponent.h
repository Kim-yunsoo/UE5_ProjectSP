// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SPInventoryComponent.generated.h"

class USPItemBase;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdated, const TArray<USPItemBase*>);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryMiniUpdated, const TArray<USPItemBase*>);




UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
	IAR_NoItemAdded UMETA(DisplayName = "No item added"), //�߰� ����
	IAR_PartialAmountItemAdded UMETA(DisplayName = "Partial amount of item added"), //�κ� 
	IAR_AllItemAdded UMETA(DisplayName = "All of item added") //��ü 
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

	FItemAddResult() :
	ActualAmountAdded(0),
	OperationResult(EItemAddResult::IAR_NoItemAdded),
	ResultMessage(FText::GetEmpty())
	{};

	// Actual amount of item that was added to the inventoy ��� �߰��� ǰ���� ���� �ݾ�
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result");
	int32 ActualAmountAdded;
	//Enum representing the end state of an add item operation �׸� �߰� �۾��� ���� ���¸� ��Ÿ���� ����
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result");
	EItemAddResult OperationResult;
	//Informational message that can be passed with the result ����� �Բ� ������ �� �ִ� ���� �޽���
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result");
	FText ResultMessage;

	static FItemAddResult AddedNone(const FText& ErrorText)
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = 0;
		AddedNoneResult.OperationResult = EItemAddResult::IAR_NoItemAdded;
		AddedNoneResult.ResultMessage = ErrorText;
		return AddedNoneResult;
	}

	static FItemAddResult AddedPartial(const int32 PartialAmountAdded, const FText& ErrorText)
	{
		FItemAddResult AddedPartialResult;
		AddedPartialResult.ActualAmountAdded = PartialAmountAdded;
		AddedPartialResult.OperationResult = EItemAddResult::IAR_PartialAmountItemAdded;
		AddedPartialResult.ResultMessage = ErrorText;
		return AddedPartialResult;
	}

	static FItemAddResult AddedAll(const int32 AmountAdded, const FText& Message)
	{
		FItemAddResult AddedAllResult;
		AddedAllResult.ActualAmountAdded = AmountAdded;
		AddedAllResult.OperationResult = EItemAddResult::IAR_AllItemAdded;
		AddedAllResult.ResultMessage = Message;
		return AddedAllResult;
	}
};

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
	FItemAddResult HandleAddItem(USPItemBase* InputItem);

	UFUNCTION(Category = "Inventory")	
	USPItemBase* FindMatchingItem(USPItemBase* ItemIn) const;

	UFUNCTION(Category = "Inventory")	
	void RemoveInventorMakeContents(USPItemBase* ItemToRemove);
	
	UFUNCTION(Category = "Inventory")
	USPItemBase* FindNextItemByID(USPItemBase* ItemIn) const;
	UFUNCTION(Category = "Inventory")
	USPItemBase* FindNextPartialStack(USPItemBase* ItemIn) const;
	UFUNCTION(Category = "Inventory")
	void RemoveSingleinstanceOfItem(USPItemBase* ItemToRemove); //인벤토리 배열에서 삭제
	UFUNCTION(Category = "Inventory")
	int32 RemoveAmountOfItem(USPItemBase* ItemIn, int32 DesiredAmountToRemove); //인벤토리는 있고 부분적으로 삭제
	UFUNCTION(Category = "Inventory")
	void SplitExistingStack(USPItemBase* ItemIn, const int32 AmountToSplit);
	
	UPROPERTY(EditInstanceOnly, Category = "Inventory | Item Initialization")
	TObjectPtr<UDataTable> ItemDataTable;
	
	UFUNCTION(Category = "Inventory")
	USPItemBase* MakingPotion();
	
	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetInventoryTotalWeight() const { return InventoryTotalWeight;}; //필요 없을 듯
	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetWeightCapactiy() const {return inventoryWeightCapacity;};
	UFUNCTION(Category = "Inventory")
	FORCEINLINE int32 GetSlotsCapacity() const {return InventorySlotsCapacity;};
	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<USPItemBase*> GetInventoryContents() const{return InventoryContents;};
	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<USPItemBase*> GetInventorMiniContents() {return InventoryMiniContents;};
	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<USPItemBase*> GetInventorMakeContents() {return InventoryMakeContents;};

	UFUNCTION(Category = "Inventory")
	FORCEINLINE void AddInventorMakeContents(USPItemBase* Item) {InventoryMakeContents.Add(Item);};
	
	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetSlotsCapacity(const int32 NewSlotsCapacity){InventorySlotsCapacity = NewSlotsCapacity;};
	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetWeightCapacity(const float NewWeightCapacity){inventoryWeightCapacity = NewWeightCapacity;};

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
	TArray<USPItemBase*> InventoryContents;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<USPItemBase *> InventoryMiniContents;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<USPItemBase *> InventoryMakeContents;
	
	FItemAddResult HandleNonStackableItems(USPItemBase* ItemIn, int32 RequestedAddAmount);
	int32 HandleStackableItems(USPItemBase* ItemIn, int32 RequestedAddAmount);
	int32 HandleStackableItemsMini(USPItemBase* ItemIn, int32 RequestedAddAmount);
	int32 CalculateWeightAddAmount(USPItemBase* ItemIn, int32 RequestedAddAmount);
	int32 CalculatenumberForFullStack(USPItemBase* StackablItem, int32 RequestedAddAmount);

	void AddNewItem(USPItemBase* Item, const int32 AmountToAdd);
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
		
};
