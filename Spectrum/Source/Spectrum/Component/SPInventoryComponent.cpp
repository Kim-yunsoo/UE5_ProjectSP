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
		int ServerCount = ItemIn->Quantity - ActualAmountToRemove;
		ItemIn->SetQuantity(ServerCount);
		UE_LOG(LogTemp, Warning, TEXT("RemoveAmountOfItem num %d"), IsMiniPotion(ItemIn->ID));
		UE_LOG(LogTemp, Warning, TEXT("RemoveAmountOfItem %d"), ServerCount);
		if(ItemIn->ItemType == EItemType::IngredientPotion)
		{
			
			ClientRPCUpdateMiniPotion(IsMiniPotion(ItemIn->ID), ServerCount);
		
		}
		else if (ItemIn->ItemType == EItemType::Potion)
		{
			UE_LOG(LogTemp, Warning, TEXT("RemoveAmountOfItem Here"));

			ClientRPCUpdatePotion(IsPotion(ItemIn->ID), ServerCount);
		}
	}
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

int USPInventoryComponent::HandleStackableItems(USPItemBase* ItemIn, int32 RequestedAddAmount)
{
	int num = IsPotion(ItemIn->ID);
	UE_LOG(LogTemp, Warning, TEXT("IsPotion %d"), num);

	//Todo 10개 제한으로 다시 바꾸기 
	if(num != -1)
	{
		int ServerCount = InventoryContents[num]->Quantity + RequestedAddAmount;
		InventoryContents[num]->SetQuantity(ServerCount);
		UE_LOG(LogTemp, Warning, TEXT("HandleStackableItems %d"), ServerCount);
		return ServerCount;
	}
	return -1;
}

int USPInventoryComponent::HandleStackableItemsMini(USPItemBase* ItemIn, int32 RequestedAddAmount)
{
	int num = IsMiniPotion(ItemIn->ID);
	//Todo 10개 제한으로 다시 바꾸기 
	if(num != -1)
	{
		int ServerCount = InventoryMiniContents[num]->Quantity + RequestedAddAmount;
		InventoryMiniContents[num]->SetQuantity(ServerCount);
		UE_LOG(LogTemp, Warning, TEXT("HandleStackableItemsMini %d"), ServerCount);

		return ServerCount;
	}
	return -1;
}

void USPInventoryComponent::HandleAddItem(USPItemBase* InputItem)
{
	SP_SUBLOG(LogSPSkill, Log, TEXT("%s"), TEXT("HandleAddItem"));

	const int32 InitialRequestedAddmount = 1;
	if (InputItem->ItemType == EItemType::IngredientPotion)
	{
		int ServerCount = HandleStackableItemsMini(InputItem, InitialRequestedAddmount);
		UE_LOG(LogTemp, Warning, TEXT("HandleAddItem %d"), ServerCount);

		ClientRPCUpdateMiniPotion(IsMiniPotion(InputItem->ID), ServerCount);
	}
	else if (InputItem->ItemType == EItemType::Potion)
	{
		int ServerCount = HandleStackableItems(InputItem, InitialRequestedAddmount);
		UE_LOG(LogTemp, Warning, TEXT("HandleAddItem %d"), ServerCount);
		ClientRPCUpdatePotion(IsPotion(InputItem->ID), ServerCount);
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

USPItemBase* USPInventoryComponent::FindMatchingMiniItem(int Num) 
{
	return InventoryMiniContents[Num];
}

USPItemBase* USPInventoryComponent::FindMatchingItem(int num) 
{
	return InventoryContents[num];
}

USPItemBase* USPInventoryComponent::FindItem(USPItemBase* ItemIn, EItemType Potion) const
{
	if (ItemIn && Potion == EItemType::IngredientPotion)
	{
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
	if(FindItem(ItemToRemove, ItemToRemove->ItemType))
	{
		InventoryMakeContents.RemoveSingle(ItemToRemove);
	}
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
	
	USPItemBase* R_Mini = InitializeInventory(USPItemBase::StaticClass(), "R_Mini");
	R_Mini->OwningInventory = this;
	InventoryMiniContents.Add(R_Mini);
	USPItemBase* Y_Mini = InitializeInventory(USPItemBase::StaticClass(), "Y_Mini");
	Y_Mini->OwningInventory = this;
	InventoryMiniContents.Add(Y_Mini);
	USPItemBase* B_Mini = InitializeInventory(USPItemBase::StaticClass(), "B_Mini");
	B_Mini->OwningInventory = this;
	InventoryMiniContents.Add(B_Mini);
}


int USPInventoryComponent::IsPotion(FName ID)
{
	for (int32 Index = 0; Index < InventoryContents.Num(); ++Index)
	{
		const USPItemBase* InventoryItem = InventoryContents[Index];
		if (InventoryItem && InventoryItem->ID == ID)
		{
			return Index;
		}
	}
	return -1;
}

int USPInventoryComponent::CountPotion(int num)
{
	return InventoryContents[num]->Quantity;
}

int USPInventoryComponent::IsMiniPotion(FName ID)
{
	for (int32 Index = 0; Index < InventoryMiniContents.Num(); ++Index)
	{
		const USPItemBase* InventoryItem = InventoryMiniContents[Index];
		if (InventoryItem && InventoryItem->ID == ID)
		{
			return Index;
		}
	}
	return -1;
}

void USPInventoryComponent::ClientRPCUpdateMiniPotion_Implementation(const int& num, const int&ServerCount)
{
	UE_LOG(LogTemp, Warning, TEXT("HERERERE!?"));
	UE_LOG(LogTemp, Warning, TEXT(" ClientRPC %d"), num);
	UE_LOG(LogTemp, Warning, TEXT(" ClientRPC %d"), ServerCount);
	InventoryMiniContents[num]->SetQuantity(ServerCount);
	OnInventoryMiniUpdated.Broadcast(InventoryMiniContents);
}

void USPInventoryComponent::ClientRPCUpdatePotion_Implementation(const int& num, const int&ServerCount)
{
	UE_LOG(LogTemp, Warning, TEXT(" ClientRPC %d"), num);
	UE_LOG(LogTemp, Warning, TEXT(" ClientRPC %d"), ServerCount);
	InventoryContents[num]->SetQuantity(ServerCount);
	OnInventoryUpdated.Broadcast(InventoryContents);

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

USPItemBase* USPInventoryComponent::FindPotionItem(FName ID)
{
	UE_LOG(LogTemp, Warning, TEXT("FINDPOTION1"));

	for(USPItemBase* InventoryItem : InventoryContents)
	{
		if (InventoryItem && InventoryItem->ID == ID)
		{
			UE_LOG(LogTemp, Warning, TEXT("FINDPOTION2"));
			return InventoryItem;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("FINDPOTION3"));
	return nullptr;
}