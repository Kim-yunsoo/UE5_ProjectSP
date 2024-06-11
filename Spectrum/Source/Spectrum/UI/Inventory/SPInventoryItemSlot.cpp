// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/SPInventoryItemSlot.h"
#include "SPItemDragDropOperation.h"
#include "Character/SPCharacterPlayer.h"
#include "Component/SPInventoryComponent.h"
#include "Components/Border.h"
#include "UI/Inventory/SPDrageItemVisual.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Potion/SPItemBase.h"
#include "UI/SPHUDWidget.h"

void USPInventoryItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void USPInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();
	
	PlayerCharacter = Cast<ASPCharacterPlayer>(GetOwningPlayerPawn());
	
	if(ItemReference)
	{
		ItemIcon->SetBrushFromTexture(ItemReference->ItemAssetData.Icon );
	}
	if(ItemReference->ItemNumericData.bIsStackable)
	{
		ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));
	}
	else
	{
		ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
	}
	ClickOn = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Spectrum/Sound/ClickOn.ClickOn'"));
}

FReply USPInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if(InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) //좌클릭 드래그 버튼 
	{
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}
	if(InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton) //우클릭 - 취소할 때 
	{
		ASPCharacterPlayer* Player = Cast<ASPCharacterPlayer>(GetOwningPlayerPawn());
		
		ClickResult = Player->BackItem(ItemReference, 1); //되돌린다. 근데 완제품을 누르면 터짐
		if(ClickResult)
		{
			this->RemoveFromParent();
		}

		
		return Reply.Handled();
	}
	return Reply.Unhandled();
}

void USPInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void USPInventoryItemSlot::HideText()
{
	ItemQuantity->SetVisibility(ESlateVisibility::Hidden);
}

FReply USPInventoryItemSlot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	if(PlayerCharacter->GetInventory()->IsPotion(ItemReference->ID) != -1 )
	{
		ASPCharacterPlayer* Player = Cast<ASPCharacterPlayer>(GetOwningPlayerPawn());
		int num = Player->GetInventory()->IsPotion(ItemReference->ID);
		Player->AddItemClick(num);
		SetVisibility(ESlateVisibility::Hidden);
		Player->HUDWidget->ClearMakingWieget();
		PlaySound(ClickOn);
		return Reply.Handled(); 
	}
	return Reply.Unhandled();
}

void USPInventoryItemSlot::ServerRPCButtonUp_Implementation(int num)
{
	ASPCharacterPlayer* Player = Cast<ASPCharacterPlayer>(GetOwningPlayerPawn());
	USPItemBase* ItemBase = Player->GetInventory()->FindMatchingItem(num);
	Player->BackItem(ItemReference, 1);
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
			DragVisual->ItemQuantity->SetVisibility(ESlateVisibility::Hidden);
			
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


