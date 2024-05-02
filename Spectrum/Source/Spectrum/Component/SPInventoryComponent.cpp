// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SPInventoryComponent.h"
#include "IDetailTreeNode.h"
#include "SpectrumLog.h"
#include "Potion/SPItemBase.h"

// Sets default values for this component's properties
USPInventoryComponent::USPInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableFinder(TEXT("/Script/Engine.DataTable'/Game/Spectrum/ItemData/Item.Item'"));
	if (DataTableFinder.Succeeded())
	{
		// ������ ���̺��� ��ȿ�ϸ� ����
		ItemDataTable = DataTableFinder.Object;
		UE_LOG(LogTemp, Warning, TEXT("DATA TEST"));
	}
	// ...
}

USPItemBase* USPInventoryComponent::FindMatchingItem(USPItemBase* ItemIn) const
{
	if (ItemIn)
	{
		UE_LOG(LogTemp,Warning, TEXT("FindMatchingItem"));
		if(InventoryContents.Contains(ItemIn) || InventoryMiniContents.Contains(ItemIn))
		{
			return ItemIn;
		}
		
	}
	return nullptr;
}

void USPInventoryComponent::RemoveInventorMakeContents(USPItemBase* ItemToRemove) 
{
	if(FindMatchingItem(ItemToRemove))
	{
		InventoryMakeContents.RemoveSingle(ItemToRemove);
	}
	//OnInventoryUpdated.Broadcast(InventoryContents);
}

USPItemBase* USPInventoryComponent::FindNextItemByID(USPItemBase* ItemIn) const
{
	if(ItemIn)
	{
		//����������
		if(const TArray<USPItemBase*>::ElementType* Result = InventoryContents.FindByKey(ItemIn))
		{
			return *Result;
		}
	}
	return nullptr;
}

USPItemBase* USPInventoryComponent::FindNextPartialStack(USPItemBase* ItemIn) const
{
	//�κ� ������ �ִ� ��� �κ��丮�� �ִ� �׸� ���캸��

	if(ItemIn->ItemType==EItemType::IngredientPotion)
	{
		if(const TArray<USPItemBase*>::ElementType* Result =
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
		if(const TArray<USPItemBase*>::ElementType* Result =
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
	
	InventoryMiniContents.RemoveSingle(ItemToRemove);

}



int32 USPInventoryComponent::RemoveAmountOfItem(USPItemBase* ItemIn, int32 DesiredAmountToRemove)
{
	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity);
	ItemIn->SetQuantity(ItemIn->Quantity- ActualAmountToRemove);
	if(ItemIn->Quantity == 0)
		RemoveSingleinstanceOfItem(ItemIn);
	OnInventoryUpdated.Broadcast(InventoryContents);
	OnInventoryMiniUpdated.Broadcast(InventoryMiniContents);
	UE_LOG(LogTemp, Warning, TEXT("RemoveAmountOfItem"));

	return ActualAmountToRemove;
}

void USPInventoryComponent::SplitExistingStack(USPItemBase* ItemIn, const int32 AmountToSplit) //���� ������ 
{
	if(!(InventoryContents.Num() + 1 > InventorySlotsCapacity))
	{
		RemoveAmountOfItem(ItemIn, AmountToSplit);
		AddNewItem(ItemIn, AmountToSplit);
	}
}

USPItemBase* USPInventoryComponent::MakingPotion()
{
	UE_LOG(LogTemp, Warning, TEXT("MakingPotion"));
	int Blue = 0;
	int Yellow = 0;
	int Red = 0;
	for(USPItemBase* Item : InventoryMakeContents)
	{
		FString ItemName = Item->ItemTextData.Name.ToString();
		if (ItemName == TEXT("BP"))
		{
			Blue++;
		}
		else if (ItemName == TEXT("YP"))
		{
			Yellow++;
		}
		else if (ItemName == TEXT("RP"))
		{
			Red++;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("blue %d Yellow %d Red %d"),Blue, Yellow, Red)
	//FName DesiredItemID = "HAPPY";
	if(Blue == 2 && Yellow == 1)
	{
		FName DesiredItemID = "G_Potion";
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());
		// ���ϴ� �׸� ã��
		USPItemBase* Item = NewObject<USPItemBase>(); 
		Item->ID = ItemData->ID;
		Item->ItemType = ItemData->ItemType;
		Item->ItemTextData = ItemData->ItemTextData;
		Item->ItemNumericData = ItemData->ItemNumericData;
		Item->ItemAssetData = ItemData->ItemAssetData;
		return Item;
	}
	else if(Blue == 1 && Red == 2)
	{
		FName DesiredItemID = "P_Potion";
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());
		// ���ϴ� �׸� ã��
		USPItemBase* Item = NewObject<USPItemBase>(); 
		Item->ID = ItemData->ID;
		Item->ItemType = ItemData->ItemType;
		Item->ItemTextData = ItemData->ItemTextData;
		Item->ItemNumericData = ItemData->ItemNumericData;
		Item->ItemAssetData = ItemData->ItemAssetData;
		return Item;
	}
	else if (Red == 1 && Yellow ==2)
	{
		FName DesiredItemID = "O_Potion";
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());
		// ���ϴ� �׸� ã��
		USPItemBase* Item = NewObject<USPItemBase>(); 
		Item->ID = ItemData->ID;
		Item->ItemType = ItemData->ItemType;
		Item->ItemTextData = ItemData->ItemTextData;
		Item->ItemNumericData = ItemData->ItemNumericData;
		Item->ItemAssetData = ItemData->ItemAssetData;
		return Item;
	}
	else
	{
		return nullptr;
	}

	
	
	//HandleAddItem(Item);
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
	if(InventoryContents.Num() + 1 > InventorySlotsCapacity) //������ �� á�� ���
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
	//Todo 10�� �������� �ٽ� �ٲٱ� 
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
	//UE_LOG(LogTemp,Warning, TEXT("here1"));
	//Todo 10�� �������� �ٽ� �ٲٱ� 
	if (Existingitem)
	{
		const int32 AmountToMakeFullStack = CalculatenumberForFullStack(Existingitem, AmountToDistribute);
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(Existingitem, AmountToMakeFullStack);
		if(WeightLimitAddAmount > 0)
		{
			Existingitem->SetQuantity(Existingitem->Quantity + WeightLimitAddAmount);
			AmountToDistribute -= WeightLimitAddAmount;
			//UE_LOG(LogTemp,Warning, TEXT("here4"));
		}
	}
	else if(InventoryMiniContents.Num() + 1 <= InventorySlotsCapacity)
	{
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ItemIn, AmountToDistribute);
		if(WeightLimitAddAmount > 0)
		{
			//UE_LOG(LogTemp,Warning, TEXT("here2"));
			AddNewItem(ItemIn, AmountToDistribute);
			return RequestedAddAmount;
		}
	}
	//UE_LOG(LogTemp,Warning, TEXT("here3"));

	OnInventoryMiniUpdated.Broadcast(InventoryMiniContents);
	return RequestedAddAmount - AmountToDistribute;
}

FItemAddResult USPInventoryComponent::HandleAddItem(USPItemBase* InputItem)
{
	if(GetOwner())
	{
		const int32 InitialRequestedAddmount = 1;

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
	
		// if(StackableAmountAdded == InitialRequestedAddmount) //��� �� �ֱ� 
		// {
		// 	//return added all result;
		// 	return FItemAddResult::AddedAll(InitialRequestedAddmount, FText::Format(
		// 		FText::FromString("Successfully add {0} {1} to the inventory."), InitialRequestedAddmount, InputItem->ItemTextData.Name));
		// }
		//
		// if(StackableAmountAdded < InitialRequestedAddmount && StackableAmountAdded > 0)
		// {
		// 	//return added partial result; //�κи� �ֱ�
		// 	return FItemAddResult::AddedPartial(InitialRequestedAddmount, FText::Format(
		// 	FText::FromString("Partial amount of {0} to the inventory. Number added = {1}"), StackableAmountAdded));
		// }
		//
		// if(StackableAmountAdded <= 0)
		// {
		// 	//return added none result; //�� ���� �ʱ�
		// 	return FItemAddResult::AddedNone(FText::Format(
		// 	FText::FromString("Couldn't add {0} to the inventory, no remaining inventory slots, or invalid item"),
		// 	InputItem->ItemTextData.Name));
		// }
		
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
		//�ٸ� �κ��丮�� �̵��� ��
		NewItem = Item->CreatePotionCopy();
	}

	NewItem->OwningInventory = this;
	NewItem->SetQuantity(AmountToAdd);

	if(Item->ItemType == EItemType::IngredientPotion)
	{
		InventoryMiniContents.Add(NewItem);
		OnInventoryMiniUpdated.Broadcast(InventoryMiniContents);
		ServerRPCUpdateMiniPotion(InventoryMiniContents);
	}
	else
	{
		InventoryContents.Add(NewItem);
		OnInventoryUpdated.Broadcast(InventoryContents);
		ServerRPCUpdatePotion(InventoryContents);
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

bool USPInventoryComponent::IsPotion(FName ID)
{
	for (const USPItemBase* InventoryItem : InventoryContents)
	{
		if (InventoryItem && InventoryItem->ID == ID)
		{
			// �������� ã������, ID�� ��ġ�ϹǷ� true�� ��ȯ�մϴ�.
			return true;
		}
	}
    
	// �������� ã�� ���߰ų� ID�� ��ġ���� �ʴ� ��� false�� ��ȯ�մϴ�.
	return false;
}

void USPInventoryComponent::ServerRPCUpdatePotion_Implementation(const TArray<USPItemBase*>& Potion)
{
	InventoryContents = Potion;
	SP_SUBLOG(LogPotion, Log, TEXT("Potion"));
}

void USPInventoryComponent::ServerRPCUpdateMiniPotion_Implementation(const TArray<USPItemBase*>& Mini)
{
	InventoryMiniContents = Mini;
	SP_SUBLOG(LogPotion, Log, TEXT("Mini"));
}

