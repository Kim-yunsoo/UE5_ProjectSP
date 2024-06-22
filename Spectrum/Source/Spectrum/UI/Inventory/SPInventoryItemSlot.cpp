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
	if(InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) //��Ŭ�� �巡�� ��ư 
	{
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton); // DetectDrag �޼��带 ȣ���ؼ� ���� �����Ѵ�. 
	}
	if(InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton) //��Ŭ�� - ����� �� 
	{
		ASPCharacterPlayer* Player = Cast<ASPCharacterPlayer>(GetOwningPlayerPawn());
		
		ClickResult = Player->BackItem(ItemReference, 1); 
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
{/// ���콺 ��ư�� �� �� ����ǰ�� Ŭ���ϰ� �� ���? 
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

void USPInventoryItemSlot::ServerRPCButtonUp_Implementation(int num) //�Ⱦ��µ��ϴ� 
{
	ASPCharacterPlayer* Player = Cast<ASPCharacterPlayer>(GetOwningPlayerPawn());
	USPItemBase* ItemBase = Player->GetInventory()->FindMatchingItem(num);
	Player->BackItem(ItemReference, 1);
}

void USPInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                                UDragDropOperation*& OutOperation) //������ �巡�� ������ �����Ǿ��� �� ȣ���ϴ� Ŭ���� �޼��� 
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if(DragItemVisualClass) //�巹�� ����  �� ���̴� ���� Ŭ������ ��ȿ���� 
	{
		const TObjectPtr<USPDrageItemVisual> DragVisual = CreateWidget<USPDrageItemVisual>(this, DragItemVisualClass);

		if(DragVisual)
		{
			DragVisual->ItemIcon->SetBrushFromTexture(ItemReference->ItemAssetData.Icon);
			DragVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());
			DragVisual->ItemQuantity->SetVisibility(ESlateVisibility::Hidden);
			
			USPItemDragDropOperation* DragItemOperation = NewObject<USPItemDragDropOperation>(); //�巹�׿� ��� �۾��� ����
			DragItemOperation->SourceItem = ItemReference; //�巹�� �� ����� ���ҽ�
			DragItemOperation->SourceInventory = ItemReference->OwningInventory;

			DragItemOperation->DefaultDragVisual = DragVisual; //�巡�� ������ ���̴� ���� 
			DragItemOperation->Pivot = EDragPivot::TopLeft; //���̴� �Ǻ�
			OutOperation = DragItemOperation; //�̺�Ʈ�� ó���� �� �ִ� ��ü�� �����ȴ�. 
		}
	}
}

bool USPInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}


