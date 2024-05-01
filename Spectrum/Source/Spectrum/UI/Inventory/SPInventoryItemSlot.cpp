// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/SPInventoryItemSlot.h"

#include "SPItemDragDropOperation.h"
#include "Character/SPCharacterPlayer.h"
#include "Components/Border.h"
#include "UI/Inventory/SPDrageItemVisual.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Potion/SPItemBase.h"

void USPInventoryItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void USPInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if(ItemReference)
	{
		ItemIcon->SetBrushFromTexture(ItemReference->ItemAssetData.Icon);
	}
	if(ItemReference->ItemNumericData.bIsStackable)
	{
		ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));
	}
	else
	{
		ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
	}
}

FReply USPInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if(InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}
	else if(InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		ASPCharacterPlayer* Player = Cast<ASPCharacterPlayer>(GetOwningPlayerPawn());
		Player->BackItem(ItemReference, 1);
		UE_LOG(LogTemp, Warning, TEXT("BACK Inventory"));
		SetVisibility(ESlateVisibility::Hidden);
		return Reply.Handled();
	}
	return Reply.Unhandled();
}
//하위 메뉴 만들 때 사용
void USPInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void USPInventoryItemSlot::HideText()
{
	ItemQuantity->SetVisibility(ESlateVisibility::Hidden);
}

void USPInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                                UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if(DragItemVisualClass)
	{
		const TObjectPtr<USPDrageItemVisual> DragVisual = CreateWidget<USPDrageItemVisual>(this, DragItemVisualClass);

		if(DragVisual)
		{
			DragVisual->ItemIcon->SetBrushFromTexture(ItemReference->ItemAssetData.Icon);
			DragVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());
			//DragVisual->ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));
			DragVisual->ItemQuantity->SetVisibility(ESlateVisibility::Hidden);
			//HideText();
			USPItemDragDropOperation* DragItemOperation = NewObject<USPItemDragDropOperation>();
			DragItemOperation->SourceItem = ItemReference;
			DragItemOperation->SourceInventory = ItemReference->OwningInventory;

			DragItemOperation->DefaultDragVisual = DragVisual;
			DragItemOperation->Pivot = EDragPivot::TopLeft;
			OutOperation = DragItemOperation;
		}
	
	}
	
}

bool USPInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}


