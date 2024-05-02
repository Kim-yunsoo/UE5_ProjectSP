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
		// 데이터 테이블이 유효하면 설정
		ItemDataTable = DataTableFinder.Object;
		UE_LOG(LogTemp, Warning, TEXT("DATA TEST"));
	}
	// ...
	InventorySlotsCapacity = 100;
	// InventoryMiniContents.SetNum(3);
	// InventoryMakeContents.SetNum(3);
	// InventoryContents[4]->ID = "G_Potion";

}

USPItemBase* USPInventoryComponent::FindMatchingItem(USPItemBase* ItemIn, EItemType Potion) const
{
	if (ItemIn && Potion == EItemType::IngredientPotion)
	{
		UE_LOG(LogTemp,Warning, TEXT("FindMatchingItem"));
		if(InventoryMiniContents.Contains(ItemIn))
		{
			return ItemIn;
		}
	}
	else
	{
		if(InventoryContents.Contains(ItemIn))
		{
			return ItemIn;
		}
	}
	return nullptr;
}

void USPInventoryComponent::RemoveInventorMakeContents(USPItemBase* ItemToRemove) 
{
	if(FindMatchingItem(ItemToRemove, ItemToRemove->ItemType))
	{
		InventoryMakeContents.RemoveSingle(ItemToRemove);
	}
	//OnInventoryUpdated.Broadcast(InventoryContents);
}

// USPItemBase* USPInventoryComponent::FindNextItemByID(USPItemBase* ItemIn) const
// {
// 	if(ItemIn && )
// 	{
// 		//이중포인터
// 		if(const TArray<USPItemBase*>::ElementType* Result = InventoryContents.FindByKey(ItemIn))
// 		{
// 			return *Result;
// 		}
// 	}
// 	return nullptr;
// }

USPItemBase* USPInventoryComponent::FindNextPartialStack(USPItemBase* ItemIn) const
{
	//부분 스택이 있는 경우 인벤토리에 있는 항목 살펴보기

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

void USPInventoryComponent::RemoveSingleinstanceOfItem(USPItemBase* ItemToRemove, EItemType Potion)
{
	if(ItemToRemove->ItemType == EItemType::IngredientPotion)
		InventoryMiniContents.RemoveSingle(ItemToRemove);
	else
	{
		InventoryContents.RemoveSingle(ItemToRemove);
	}
}



int32 USPInventoryComponent::RemoveAmountOfItem(USPItemBase* ItemIn, int32 DesiredAmountToRemove)
{
	if(GetOwner()->HasAuthority())
	{
		const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity);
		ItemIn->SetQuantity(ItemIn->Quantity - ActualAmountToRemove);
		if (ItemIn->Quantity == 0 && ItemIn->ItemType == EItemType::IngredientPotion)
		{
			//RemoveSingleinstanceOfItem(ItemIn, ItemIn->ItemType);
			UE_LOG(LogTemp, Warning, TEXT("RemoveAmountOfItem1"));
		}
		else if (ItemIn->Quantity == 0 && ItemIn->ItemType == EItemType::Potion)
		{
			//RemoveSingleinstanceOfItem(ItemIn, ItemIn->ItemType);
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetName());
		OnInventoryUpdated.Broadcast(InventoryContents);
		OnInventoryMiniUpdated.Broadcast(InventoryMiniContents);
		//ClientRPCRemovePotion(ItemIn, 1);
	}
	// else
	// {
	// 	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity);
	// 	ItemIn->SetQuantity(ItemIn->Quantity - ActualAmountToRemove);
	// 	if (ItemIn->Quantity == 0 && ItemIn->ItemType == EItemType::IngredientPotion)
	// 	{
	// 		RemoveSingleinstanceOfItem(ItemIn, ItemIn->ItemType);
	// 		UE_LOG(LogTemp, Warning, TEXT("RemoveAmountOfItem1"));
	// 	}
	// 	else if (ItemIn->Quantity == 0 && ItemIn->ItemType == EItemType::Potion)
	// 	{
	// 		RemoveSingleinstanceOfItem(ItemIn, ItemIn->ItemType);
	// 	}
	// 	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetName());
	// 	OnInventoryUpdated.Broadcast(InventoryContents);
	// 	OnInventoryMiniUpdated.Broadcast(InventoryMiniContents);
	// }
	
	
	return 0;
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
		// 원하는 항목 찾기
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
		// 원하는 항목 찾기
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
		// 원하는 항목 찾기
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

}


void USPInventoryComponent::HandleStackableItems(USPItemBase* ItemIn, int32 RequestedAddAmount)
{
	int num = IsPotion(ItemIn->ID);
	//Todo 10개 제한으로 다시 바꾸기 
	if(num)
	{
		InventoryContents[num]->Quantity += 1;
	}
	
}

void USPInventoryComponent::HandleStackableItemsMini(USPItemBase* ItemIn, int32 RequestedAddAmount)
{
	//AddNewItem(ItemIn, 1);
	int32 AmountToDistribute = RequestedAddAmount;
	//USPItemBase* Existingitem = FindNextPartialStack(ItemIn);
	//UE_LOG(LogTemp,Warning, TEXT("here1"));
	//Todo 10개 제한으로 다시 바꾸기 
	// if (Existingitem)
	// {
	// 	Existingitem->SetQuantity(Existingitem->Quantity + RequestedAddAmount);
	// 	//UE_LOG(LogTemp,Warning, TEXT("here4"));
	// }
	// else if(InventoryMiniContents.Num() + 1 <= InventorySlotsCapacity)
	// {
		//UE_LOG(LogTemp,Warning, TEXT("here2"));
		AddNewItem(ItemIn, AmountToDistribute);
		return ;
	//}
	// UE_LOG(LogTemp,Warning, TEXT("here3"));
	// ClientRPCUpdatePotion(InventoryMiniContents,ItemIn->ItemType);
	// OnInventoryMiniUpdated.Broadcast(InventoryMiniContents);
}

void USPInventoryComponent::HandleAddItem(USPItemBase* InputItem)
{
	SP_SUBLOG(LogSPSkill, Log, TEXT("%s"), TEXT("HandleAddItem"));
	if(GetOwner()->HasAuthority())
	{
		const int32 InitialRequestedAddmount = 1;
		if(InputItem->ItemType == EItemType::IngredientPotion)
		{
			HandleStackableItemsMini(InputItem, InitialRequestedAddmount);
			OnInventoryMiniUpdated.Broadcast(InventoryMiniContents);
		}
		else if (InputItem->ItemType == EItemType::Potion)
		{
			HandleStackableItems(InputItem, InitialRequestedAddmount);
			OnInventoryUpdated.Broadcast(InventoryContents);
			ClientRPCUpdatePotion(IsPotion(InputItem->ID));

		}
	}
}

USPItemBase* USPInventoryComponent::InitializeInventory(const TSubclassOf<USPItemBase> BaseClass, FName DesiredItemID)
{
	if (ItemDataTable && !DesiredItemID.IsNone())
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());
		// 원하는 항목 찾기

		 USPItemBase* ItemReference = NewObject<USPItemBase>(this, BaseClass);

		ItemReference->ID = ItemData->ID;
		ItemReference->ItemType = ItemData->ItemType;
		ItemReference->ItemTextData = ItemData->ItemTextData;
		ItemReference->ItemNumericData = ItemData->ItemNumericData;
		ItemReference->ItemAssetData = ItemData->ItemAssetData;
		ItemReference->SetQuantity(0);
		return ItemReference;
	}
	return nullptr;
}

void USPInventoryComponent::AddNewItem(USPItemBase* Item, const int32 AmountToAdd)
{
	// USPItemBase* NewItem;
	// if(Item->bIsCopy || Item->bIsPickup)
	// {
	// 	NewItem = Item;
	// 	NewItem->ResetItemFlags();
	// }
	// else
	// {
	// 	//다른 인벤토리로 이동할 때
	// 	NewItem = Item->CreatePotionCopy();
	// }
	// Item->OwningInventory = this;
	// Item->SetQuantity(AmountToAdd);
	// int num = IsPotion(Item->ID); 
	// if(Item->ItemType == EItemType::IngredientPotion)
	// {
	// 	InventoryMiniContents.Add(NewItem);
	// }
	// else if(Item->ItemType == EItemType::Potion)
	// {
	// 	InventoryContents.Add(NewItem);
	// }
}

USPItemBase* USPInventoryComponent::FindPotionItem(FName ID)
{
	for(USPItemBase* InventoryItem : InventoryContents)
	{
		if (InventoryItem && InventoryItem->ID == ID)
		{
			return InventoryItem;
		}
	}
	return nullptr;
}

USPItemBase* USPInventoryComponent::FindMiniItem(FName ID)
{
	for(USPItemBase* InventoryItem : InventoryMiniContents)
	{
		if (InventoryItem && InventoryItem->ID == ID)
		{
			return InventoryItem;
		}
	}
	return nullptr;
}

// Called when the game starts
void USPInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	USPItemBase* G_Potion = InitializeInventory(USPItemBase::StaticClass(), "G_Potion");
	G_Potion->OwningInventory = this;
	InventoryContents.Add(G_Potion);
	USPItemBase* O_Potion = InitializeInventory(USPItemBase::StaticClass(), "O_Potion");
	O_Potion->OwningInventory = this;
	InventoryContents.Add(O_Potion);
	USPItemBase* P_Potion = InitializeInventory(USPItemBase::StaticClass(), "P_Potion");
	P_Potion->OwningInventory = this;
	InventoryContents.Add(P_Potion);
	USPItemBase* B_Potion = InitializeInventory(USPItemBase::StaticClass(), "B_Potion");
	B_Potion->OwningInventory = this;
	InventoryContents.Add(B_Potion);
}


// Called every frame
void USPInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int USPInventoryComponent::IsPotion(FName ID)
{
	for (int32 Index = 0; Index < InventoryContents.Num(); ++Index)
	{
		const USPItemBase* InventoryItem = InventoryContents[Index];
		if (InventoryItem && InventoryItem->ID == ID)
		{
			// 아이템이 찾아졌고, ID가 일치하므로 해당 아이템의 인덱스를 반환합니다.
			return Index;
		}
	}
    
	// 아이템을 찾지 못했거나 ID가 일치하지 않는 경우 false를 반환합니다.
	return -1;
}

void USPInventoryComponent::ClientRPCRemovePotion_Implementation(const int& num)
{
	InventoryContents[num]->Quantity += 1;
}

void USPInventoryComponent::ClientRPCUpdatePotion_Implementation(const int& num)
{
	SP_SUBLOG(LogSPSkill, Log, TEXT("%s"), TEXT("Client"));

	InventoryContents[num]->Quantity += 1;
	OnInventoryUpdated.Broadcast(InventoryContents);
}
