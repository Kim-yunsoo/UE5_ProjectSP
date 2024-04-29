// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SPInventoryComponent.h"
#include "IDetailTreeNode.h"
#include "Potion/SPItemBase.h"

// Sets default values for this component's properties
USPInventoryComponent::USPInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

USPItemBase* USPInventoryComponent::FindMatchingItem(USPItemBase* ItemIn) const
{
	if (ItemIn)
	{
		if(InventoryContents.Contains(ItemIn))
		{
			return ItemIn;
		}
	}
	return nullptr;
}

USPItemBase* USPInventoryComponent::FindNextItemByID(USPItemBase* ItemIn) const
{
	if(ItemIn)
	{
		//이중포인터
		if(const TArray<TObjectPtr<USPItemBase>>::ElementType* Result = InventoryContents.FindByKey(ItemIn))
		{
			return *Result;
		}
	}
	return nullptr;
}

USPItemBase* USPInventoryComponent::FindNextPartialStack(USPItemBase* ItemIn) const
{
	//부분 스택이 있는 경우 인벤토리에 있는 항목 살펴보기

	if(ItemIn->ItemType==EItemType::IngredientPotion)
	{
		if(const TArray<TObjectPtr<USPItemBase>>::ElementType* Result =
		InventoryMiniContents.FindByPredicate([&ItemIn](const USPItemBase* InventoryItem)
		{
			return InventoryItem->ID == ItemIn->ID; //&& !InventoryItem->IsFullItemStack();
		}
		))
		{
			return *Result;
		}
	}
	else
	{
		if(const TArray<TObjectPtr<USPItemBase>>::ElementType* Result =
		InventoryContents.FindByPredicate([&ItemIn](const USPItemBase* InventoryItem)
		{
			return InventoryItem->ID == ItemIn->ID; //&& !InventoryItem->IsFullItemStack();
		}
		))
		{
			return *Result;
		}
	}
	
	return nullptr;
}

void USPInventoryComponent::RemoveSingleinstanceOfItem(USPItemBase* ItemToRemove)
{
	InventoryContents.RemoveSingle(ItemToRemove);
	OnInventoryUpdated.Broadcast(InventoryContents);
}

int32 USPInventoryComponent::RemoveAmountOfItem(USPItemBase* ItemIn, int32 DesiredAmountToRemove)
{
	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity);
	ItemIn->SetQuantity(ItemIn->Quantity- ActualAmountToRemove);
	OnInventoryUpdated.Broadcast(InventoryContents);

	return ActualAmountToRemove;
}

void USPInventoryComponent::SplitExistingStack(USPItemBase* ItemIn, const int32 AmountToSplit) //물약 나누기 
{
	if(!(InventoryContents.Num() + 1 > InventorySlotsCapacity))
	{
		RemoveAmountOfItem(ItemIn, AmountToSplit);
		AddNewItem(ItemIn, AmountToSplit);
	}
}


int32 USPInventoryComponent::CalculateWeightAddAmount(USPItemBase* ItemIn, int32 RequestedAddAmount)
{
	//const int32 WeightMaxAddAmount = FMath::FloorToInt((GetWeightCapactiy() - InventoryTotalWeight)/ItemIn->GetIm)
	
	return 1; 
}

int32 USPInventoryComponent::CalculatenumberForFullStack(USPItemBase* StackablItem, int32 InirialRequestedAddAmount)
{
	//AddNewItem(StackablItem, InirialRequestedAddAmount);
	return 10;
}


FItemAddResult USPInventoryComponent::HandleNonStackableItems(USPItemBase* ItemIn, int32 RequestedAddAmount)
{
	if(InventoryContents.Num() + 1 > InventorySlotsCapacity) //슬롯이 다 찼을 경우
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString ("Could not add {0} to the inventory. All inventory slots are full."), ItemIn->ItemTextData.Name));
	}
	AddNewItem(ItemIn, 1);
	return FItemAddResult::AddedAll(1, FText::Format(
		FText::FromString("Successfully added {0} {1}to the inventroy."), RequestedAddAmount, ItemIn->ItemTextData.Name));
	//return added all result;
}

int32 USPInventoryComponent::HandleStackableItems(USPItemBase* ItemIn, int32 RequestedAddAmount)
{
	//AddNewItem(ItemIn, 1);
	int32 AmountToDistribute = RequestedAddAmount;

	USPItemBase* Existingitem = FindNextPartialStack(ItemIn);
	//Todo 10개 제한으로 다시 바꾸기 
	while(Existingitem)
	{
		// const int32 AmountToMakeFullStack = CalculatenumberForFullStack(Existingitem, AmountToDistribute);
		// const int32 WeightLimitAddAmount = CalculateWeightAddAmount(Existingitem, AmountToMakeFullStack);

		const int32 WeightLimitAddAmount = 1;
		if(WeightLimitAddAmount > 0)
		{
			UE_LOG(LogTemp,Warning, TEXT("here1"));
			Existingitem->SetQuantity(Existingitem->Quantity + WeightLimitAddAmount);

			AmountToDistribute -= WeightLimitAddAmount;

			ItemIn->SetQuantity(AmountToDistribute);

			if(InventoryTotalWeight >= inventoryWeightCapacity)
			{
				OnInventoryUpdated.Broadcast(InventoryContents);
				return RequestedAddAmount - AmountToDistribute;
			}
		}
		// else if (WeightLimitAddAmount <= 0)
		// {
		// 	UE_LOG(LogTemp,Warning, TEXT("2"));
		//
		// 	if(AmountToDistribute != RequestedAddAmount)
		// 	{
		// 		OnInventoryUpdated.Broadcast(InventoryContents);
		// 		return RequestedAddAmount - AmountToDistribute;
		// 	}
		// 	return 0;
		// }
		if(AmountToDistribute <= 0)
		{
			UE_LOG(LogTemp,Warning, TEXT("3"));
			OnInventoryUpdated.Broadcast(InventoryContents);
			return RequestedAddAmount;
		}
		Existingitem = FindNextPartialStack(ItemIn);
	}
	if(InventoryContents.Num() + 1 <= InventorySlotsCapacity)
	{
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ItemIn, AmountToDistribute);
		if(WeightLimitAddAmount > 0)
		{
			UE_LOG(LogTemp,Warning, TEXT("here1111"));
			AddNewItem(ItemIn, AmountToDistribute);
			return RequestedAddAmount;
		}
	}

	OnInventoryUpdated.Broadcast(InventoryContents);
	return RequestedAddAmount - AmountToDistribute;
}


int32 USPInventoryComponent::HandleStackableItemsMini(USPItemBase* ItemIn, int32 RequestedAddAmount)
{
	//AddNewItem(ItemIn, 1);
	int32 AmountToDistribute = RequestedAddAmount;
	USPItemBase* Existingitem = FindNextPartialStack(ItemIn);
	//Todo 10개 제한으로 다시 바꾸기 
	while(Existingitem)
	{
		const int32 AmountToMakeFullStack = CalculatenumberForFullStack(Existingitem, AmountToDistribute);
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(Existingitem, AmountToMakeFullStack);
		if(WeightLimitAddAmount > 0)
		{
			Existingitem->SetQuantity(Existingitem->Quantity + WeightLimitAddAmount);

			AmountToDistribute -= WeightLimitAddAmount;
			ItemIn->SetQuantity(AmountToDistribute);
			if(InventoryTotalWeight >= inventoryWeightCapacity)
			{
				OnInventoryMiniUpdated.Broadcast(InventoryMiniContents);
				return RequestedAddAmount - AmountToDistribute;
			}
		}
		else if (WeightLimitAddAmount <= 0)
		{
			if(AmountToDistribute != RequestedAddAmount)
			{
				OnInventoryMiniUpdated.Broadcast(InventoryMiniContents);
				return RequestedAddAmount - AmountToDistribute;
			}
			return 0;
		}
		if(AmountToDistribute <= 0)
		{
			OnInventoryMiniUpdated.Broadcast(InventoryMiniContents);
			return RequestedAddAmount;
		}
		Existingitem = FindNextPartialStack(ItemIn);
	}
	if(InventoryMiniContents.Num() + 1 <= InventorySlotsCapacity)
	{
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ItemIn, AmountToDistribute);
		if(WeightLimitAddAmount > 0)
		{
			UE_LOG(LogTemp,Warning, TEXT("here2"));
			AddNewItem(ItemIn, AmountToDistribute);
			return RequestedAddAmount;
		}
	}

	OnInventoryMiniUpdated.Broadcast(InventoryMiniContents);
	return RequestedAddAmount - AmountToDistribute;
}

FItemAddResult USPInventoryComponent::HandleAddItem(USPItemBase* InputItem)
{
	if(GetOwner())
	{
		const int32 InitialRequestedAddmount = InputItem->Quantity;

		//쌓을 수 없는 것
		if(!InputItem->ItemNumericData.bIsStackable)
		{
			return HandleNonStackableItems(InputItem, InitialRequestedAddmount);
		}

		//쌓을 수 있는 것
		int32 StackableAmountAdded = 0;
		if(InputItem->ItemType == EItemType::IngredientPotion)
		{
			StackableAmountAdded = HandleStackableItemsMini(InputItem, InitialRequestedAddmount);
			UE_LOG(LogTemp, Warning, TEXT("TEST"));
		}
		else
		{
			StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestedAddmount);
		}
	
		if(StackableAmountAdded == InitialRequestedAddmount) //모두 다 넣기 
		{
			//return added all result;
			return FItemAddResult::AddedAll(InitialRequestedAddmount, FText::Format(
				FText::FromString("Successfully add {0} {1} to the inventory."), InitialRequestedAddmount, InputItem->ItemTextData.Name));
		}

		if(StackableAmountAdded < InitialRequestedAddmount && StackableAmountAdded > 0)
		{
			//return added partial result; //부분만 넣기
			return FItemAddResult::AddedPartial(InitialRequestedAddmount, FText::Format(
			FText::FromString("Partial amount of {0} to the inventory. Number added = {1}"), StackableAmountAdded));
		}

		if(StackableAmountAdded <= 0)
		{
			//return added none result; //다 넣지 않기
			return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Couldn't add {0} to the inventory, no remaining inventory slots, or invalid item"),
			InputItem->ItemTextData.Name));
		}
		
	}
	//check(false);
	return FItemAddResult::AddedNone(FText::FromString("TryAddItem fallthrough error. GetOwner() check somehow failed."));
}

void USPInventoryComponent::AddNewItem(USPItemBase* Item, const int32 AmountToAdd)
{
	USPItemBase* NewItem;

	if(Item->bIsCopy || Item->bIsPickup)
	{
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{
		//다른 인벤토리로 이동할 때
		NewItem = Item->CreatePotionCopy();
	}

	NewItem->OwningInventory = this;
	NewItem->SetQuantity(AmountToAdd);

	if(Item->ItemType == EItemType::IngredientPotion)
	{
		InventoryMiniContents.Add(NewItem);
		OnInventoryMiniUpdated.Broadcast(InventoryMiniContents);
	}
	else
	{
		InventoryContents.Add(NewItem);
		OnInventoryUpdated.Broadcast(InventoryContents);
	}
	
}

// Called when the game starts
void USPInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USPInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

