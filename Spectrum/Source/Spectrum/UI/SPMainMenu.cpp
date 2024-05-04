// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPMainMenu.h"
#include "UI/Inventory/SPItemDragDropOperation.h"
#include "Character/SPCharacterPlayer.h"

void USPMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void USPMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<ASPCharacterPlayer>(GetOwningPlayerPawn());
}

bool USPMainMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{

	//FDragDropEvent �巡�׸� �� �� �����Ͱ� ������

	//cast operation to item drag drop, ensure player is valid, call drop item on player

	//
	// const USPItemDragDropOperation* ItemDragDrop = Cast<USPItemDragDropOperation>(InOperation);
	//
	// if(PlayerCharacter && ItemDragDrop->SourceItem)
	// {
	// 	PlayerCharacter->DropItem(ItemDragDrop->SourceItem, ItemDragDrop->SourceItem->Quantity);
	// 	return true;
	// }
	return false;
}
