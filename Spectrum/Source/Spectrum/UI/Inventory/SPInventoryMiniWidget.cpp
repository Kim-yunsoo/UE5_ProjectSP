// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/SPInventoryMiniWidget.h"
#include "SPInventoryItemSlot.h"
#include "SPItemDragDropOperation.h"
#include "Character/SPCharacterPlayer.h"
#include "Component/SPInventoryComponent.h"
#include "Components/WrapBox.h"
#include "Potion/SPItemBase.h"

class USPItemDragDropOperation;

void USPInventoryMiniWidget::RefreshMiniInventory(TArray<USPItemBase*> Inventory) //델리게이트에 연결된 것 
{
	
	if(InventoryReference && InventorySlotClass)
	{
		InventoryPanel->ClearChildren();

		for(USPItemBase* const& InventoryItem : InventoryReference->GetInventorMiniContents())
		{
				USPInventoryItemSlot* ItemSlot = CreateWidget<USPInventoryItemSlot>(this, InventorySlotClass);
				ItemSlot->SetItemReference(InventoryItem);
				InventoryPanel->AddChildToWrapBox(ItemSlot);
		}
		SetInfoText();
	}
}

void USPInventoryMiniWidget::SetInfoText()
{
}

void USPInventoryMiniWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	PlayerCharacter = Cast<ASPCharacterPlayer>(GetOwningPlayerPawn());
	if(PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventory();
		if(InventoryReference)
		{
			InventoryReference->OnInventoryMiniUpdated.AddUObject(this, &USPInventoryMiniWidget::RefreshMiniInventory);
			SetInfoText();
		}
	}
}

bool USPInventoryMiniWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	
	 const USPItemDragDropOperation* ItemDragDrop = Cast<USPItemDragDropOperation>(InOperation);
	
	 if(PlayerCharacter && ItemDragDrop->SourceItem)
	 {
	 	return true;
	 }
	 return false;
}
