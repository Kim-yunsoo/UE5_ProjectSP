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
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton); // DetectDrag 메서드를 호출해서 동작 감지한다. 
	}
	if(InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton) //우클릭 - 취소할 때 
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
{/// 마우스 버튼을 땔 때 완제품을 클릭하고 땐 경우? 
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

void USPInventoryItemSlot::ServerRPCButtonUp_Implementation(int num) //안쓰는듯하다 
{
	ASPCharacterPlayer* Player = Cast<ASPCharacterPlayer>(GetOwningPlayerPawn());
	USPItemBase* ItemBase = Player->GetInventory()->FindMatchingItem(num);
	Player->BackItem(ItemReference, 1);
}

void USPInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                                UDragDropOperation*& OutOperation) //위에서 드래그 동작이 감지되었을 때 호출하는 클래스 메서드 
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if(DragItemVisualClass) //드레그 했을  때 보이는 위젯 클래스가 유효한지 
	{
		const TObjectPtr<USPDrageItemVisual> DragVisual = CreateWidget<USPDrageItemVisual>(this, DragItemVisualClass);

		if(DragVisual)
		{
			DragVisual->ItemIcon->SetBrushFromTexture(ItemReference->ItemAssetData.Icon);
			DragVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());
			DragVisual->ItemQuantity->SetVisibility(ESlateVisibility::Hidden);
			
			USPItemDragDropOperation* DragItemOperation = NewObject<USPItemDragDropOperation>(); //드레그엔 드롭 작업을 관리
			DragItemOperation->SourceItem = ItemReference; //드레그 앤 드롭의 리소스
			DragItemOperation->SourceInventory = ItemReference->OwningInventory;

			DragItemOperation->DefaultDragVisual = DragVisual; //드래그 했을때 보이는 위젯 
			DragItemOperation->Pivot = EDragPivot::TopLeft; //보이는 피봇
			OutOperation = DragItemOperation; //이벤트를 처리할 수 있는 객체가 설정된다. 
		}
	}
}

bool USPInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}


