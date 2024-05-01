// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/SPInventoryWidget.h"

#include "SPInventoryItemSlot.h"
#include "Character/SPCharacterPlayer.h"
#include "Component/SPInventoryComponent.h"
#include "Components/WrapBox.h"
#include "Potion/SPItemBase.h"

void USPInventoryWidget::RefreshInventory(TArray<TObjectPtr<USPItemBase>> Inventory)
{
	if(InventoryReference && InventorySlotClass)
	{
		InventoryPanel->ClearChildren();

		for(USPItemBase* const& InventoryItem : InventoryReference->GetInventoryContents())
		{
			USPInventoryItemSlot* ItemSlot = CreateWidget<USPInventoryItemSlot>(this, InventorySlotClass);
			ItemSlot->SetItemReference(InventoryItem);
			InventoryPanel->AddChildToWrapBox(ItemSlot);
		}
		SetInfoText();
	}
	
}

void USPInventoryWidget::SetInfoText()
{
	//무게 설정
}

void USPInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<ASPCharacterPlayer>(GetOwningPlayerPawn());
	if(PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventory();
		if(InventoryReference)
		{
			InventoryReference->OnInventoryUpdated.AddUObject(this, &USPInventoryWidget::RefreshInventory);
			SetInfoText();
		}
	}
}

bool USPInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
