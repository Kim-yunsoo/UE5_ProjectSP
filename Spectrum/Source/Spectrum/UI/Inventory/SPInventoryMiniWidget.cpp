// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/SPInventoryMiniWidget.h"

#include "SPInventoryItemSlot.h"
#include "Character/SPCharacterPlayer.h"
#include "Component/SPInventoryComponent.h"
#include "Components/WrapBox.h"
#include "Potion/SPItemBase.h"

void USPInventoryMiniWidget::RefreshMiniInventory(TArray<TObjectPtr<USPItemBase>> Inventory)
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
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
