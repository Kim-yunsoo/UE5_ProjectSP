// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SPInventoryComponent.h"
#include "Potion/SPItemBase.h"

// Sets default values for this component's properties
USPInventoryComponent::USPInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableFinder(
		TEXT("/Script/Engine.DataTable'/Game/Spectrum/ItemData/Item.Item'"));
	if (DataTableFinder.Succeeded())
	{
		// 데이터 테이블이 유효하면 설정
		ItemDataTable = DataTableFinder.Object;
	}
	// ...
	InventorySlotsCapacity = 100;
	//GetPotionName
	ItemCountMap.FindOrAdd(GetPotionName(EPotionType::BluePotion)) = 0;
	ItemCountMap.FindOrAdd(GetPotionName(EPotionType::RedPotion)) = 0;
	ItemCountMap.FindOrAdd(GetPotionName(EPotionType::YellowPotion)) = 0;

	SinglePotion = 1;
	DoublePotion = 2;
}


void USPInventoryComponent::RemoveSingleinstanceOfItem(USPItemBase* ItemToRemove, EItemType Potion)
{
	if (ItemToRemove->ItemType == EItemType::IngredientPotion)
		InventoryMiniContents.RemoveSingle(ItemToRemove);
	else
	{
		InventoryContents.RemoveSingle(ItemToRemove);
	}
}


int32 USPInventoryComponent::RemoveAmountOfItem(USPItemBase* ItemIn, int32 DesiredAmountToRemove)
{
	if (GetOwner()->HasAuthority())
	{
		const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity);
		int ServerCount = ItemIn->Quantity - ActualAmountToRemove;
		ItemIn->SetQuantity(ServerCount);
		if (ItemIn->ItemType == EItemType::IngredientPotion)
		{
			ClientRPCUpdatePotion(IsMiniPotion(ItemIn->ID), ServerCount, ItemIn->ItemType);
		}
		else if (ItemIn->ItemType == EItemType::Potion)
		{
			ClientRPCUpdatePotion(IsPotion(ItemIn->ID), ServerCount, ItemIn->ItemType);
		}
	}
	return 0;
}

USPItemBase* USPInventoryComponent::MakingPotion()
{
	ItemCountMap.FindOrAdd(GetPotionName(EPotionType::BluePotion)) = 0;
	ItemCountMap.FindOrAdd(GetPotionName(EPotionType::RedPotion)) = 0;
	ItemCountMap.FindOrAdd(GetPotionName(EPotionType::YellowPotion)) = 0;
	for (USPItemBase* Item : InventoryMakeContents)
	{
		FString ItemNameString = Item->ItemTextData.Name.ToString();
		FName ItemName = FName(*ItemNameString);
		if (ItemCountMap.Contains(ItemName))
		{
			ItemCountMap[ItemName]++;
		}
	}
	ItemCountMap[GetPotionName(EPotionType::BluePotion)];
	if (ItemCountMap[GetPotionName(EPotionType::BluePotion)] == DoublePotion && ItemCountMap[GetPotionName(
		EPotionType::YellowPotion)] == SinglePotion)
	{
		FName DesiredItemID = GetPotionName(EPotionType::GreenPotion);
		return CreateItemFromDataTable(DesiredItemID);
	}
	else if (ItemCountMap[GetPotionName(EPotionType::BluePotion)] == SinglePotion && ItemCountMap[GetPotionName(
		EPotionType::RedPotion)] == DoublePotion)
	{
		FName DesiredItemID = GetPotionName(EPotionType::PurplePotion);
		return CreateItemFromDataTable(DesiredItemID);
	}
	else if (ItemCountMap[GetPotionName(EPotionType::RedPotion)] == SinglePotion&& ItemCountMap[GetPotionName(
		EPotionType::YellowPotion)] == DoublePotion)
	{
		FName DesiredItemID = GetPotionName(EPotionType::OrangePotion);
		return CreateItemFromDataTable(DesiredItemID);
	}
	return nullptr;
}

USPItemBase* USPInventoryComponent::CreateItemFromDataTable(const FName& InDesiredItemID)
{
	const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(InDesiredItemID, InDesiredItemID.ToString());
	USPItemBase* Item = NewObject<USPItemBase>();
	Item->ID = ItemData->ID;
	Item->ItemType = ItemData->ItemType;
	Item->ItemTextData = ItemData->ItemTextData;
	Item->ItemNumericData = ItemData->ItemNumericData;
	Item->ItemAssetData = ItemData->ItemAssetData;
	Item->Quantity = 1;
	return Item;
}

int USPInventoryComponent::HandleStackableItems(USPItemBase* ItemIn, int32 RequestedAddAmount,
                                                const EItemType& InItemType)
{
	if (InItemType == EItemType::Potion)
	{
		int Index = IsPotion(ItemIn->ID);
		if (Index != -1) // 잘못된 아이디를 받지 않도록 검사한다. 
		{
			int PotionQuntity = InventoryContents[Index]->Quantity + RequestedAddAmount;
			InventoryContents[Index]->SetQuantity(PotionQuntity);
			return PotionQuntity;
		}
	}
	if (InItemType == EItemType::IngredientPotion)
	{
		int IngredientIndex = IsMiniPotion(ItemIn->ID);
		if (IngredientIndex != -1)
		{
			int IngredientPotion = InventoryMiniContents[IngredientIndex]->Quantity + RequestedAddAmount;
			InventoryMiniContents[IngredientIndex]->SetQuantity(IngredientPotion);
			return IngredientPotion;
		}
	}
	return -1;
}


void USPInventoryComponent::HandleAddItem(USPItemBase* InputItem, int InitialRequestedAddmount)
{
	int PotionNum = HandleStackableItems(InputItem, InitialRequestedAddmount, InputItem->ItemType);
	if (InputItem->ItemType == EItemType::Potion)
	{
		ClientRPCUpdatePotion(IsPotion(InputItem->ID), PotionNum, InputItem->ItemType);
	}
	else if (InputItem->ItemType == EItemType::IngredientPotion)
	{
		ClientRPCUpdatePotion(IsMiniPotion(InputItem->ID), PotionNum, InputItem->ItemType);
	}
}

USPItemBase* USPInventoryComponent::InitializeInventory(const TSubclassOf<USPItemBase> BaseClass, FName DesiredItemID)
{
	if (ItemDataTable && !DesiredItemID.IsNone())
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

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
		if (InventoryMiniContents.Contains(ItemIn))
		{
			return ItemIn;
		}
	}
	else
	{
		if (InventoryContents.Contains(ItemIn))
		{
			return ItemIn;
		}
	}
	return nullptr;
}

void USPInventoryComponent::RemoveInventorMakeContents(USPItemBase* ItemToRemove)
{
	if (FindItem(ItemToRemove, ItemToRemove->ItemType))
	{
		InventoryMakeContents.RemoveSingle(ItemToRemove);
	}
}

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
	USPItemBase* S_Potion = InitializeInventory(USPItemBase::StaticClass(), "S_Potion");
	S_Potion->OwningInventory = this;
	InventoryContents.Add(S_Potion);

	for (int i = 0; i < InventoryContents.Num(); ++i)
		InventoryContents[i]->Quantity = 0;

	USPItemBase* R_Mini = InitializeInventory(USPItemBase::StaticClass(), "R_Mini");
	R_Mini->OwningInventory = this;
	InventoryMiniContents.Add(R_Mini);
	USPItemBase* Y_Mini = InitializeInventory(USPItemBase::StaticClass(), "Y_Mini");
	Y_Mini->OwningInventory = this;
	InventoryMiniContents.Add(Y_Mini);
	USPItemBase* B_Mini = InitializeInventory(USPItemBase::StaticClass(), "B_Mini");
	B_Mini->OwningInventory = this;
	InventoryMiniContents.Add(B_Mini);

	for (int i = 0; i < InventoryMiniContents.Num(); ++i)
		InventoryMiniContents[i]->Quantity = 0;

	OnInventoryUpdated.Broadcast(InventoryContents);
	OnInventoryMiniUpdated.Broadcast(InventoryMiniContents);
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

FName USPInventoryComponent::GetPotionName(const EPotionType& InPotionType)
{
	switch (InPotionType)
	{
	case EPotionType::BluePotion:
		return FName("Blue Potion");
	case EPotionType::RedPotion:
		return FName("Red Potion");
	case EPotionType::YellowPotion:
		return FName("Yellow Potion");
	case EPotionType::GreenPotion:
		return FName("G_Potion");
	case EPotionType::OrangePotion:
		return FName("O_Potion");
	case EPotionType::PurplePotion:
		return FName("P_Potion");
	default:
		return FName("Nullptr");
	}
}


void USPInventoryComponent::ClientRPCUpdateMiniPotion_Implementation(const int& num, const int& ServerCount)
{
	InventoryMiniContents[num]->SetQuantity(ServerCount);
	OnInventoryMiniUpdated.Broadcast(InventoryMiniContents);
}

void USPInventoryComponent::ClientRPCUpdatePotion_Implementation(const int& num, const int& ServerCount,
                                                                 const EItemType& InItemType)
{
	if (InItemType == EItemType::Potion)
	{
		InventoryContents[num]->SetQuantity(ServerCount);
		OnInventoryUpdated.Broadcast(InventoryContents);
	}
	else if (InItemType == EItemType::IngredientPotion)
	{
		InventoryMiniContents[num]->SetQuantity(ServerCount);
		OnInventoryMiniUpdated.Broadcast(InventoryMiniContents);
	}
}

USPItemBase* USPInventoryComponent::FindMiniItem(FName ID)
{
	for (USPItemBase* InventoryItem : InventoryMiniContents)
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
	for (USPItemBase* InventoryItem : InventoryContents)
	{
		if (InventoryItem && InventoryItem->ID == ID)
		{
			return InventoryItem;
		}
	}
	return nullptr;
}
