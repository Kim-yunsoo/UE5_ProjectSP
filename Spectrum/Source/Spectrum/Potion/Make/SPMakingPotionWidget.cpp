// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion/Make/SPMakingPotionWidget.h"

#include "SPMakePotion.h"
#include "Character/SPCharacterPlayer.h"
#include "Components/WrapBox.h"

class ASPMakePotion;
class USPInventoryItemSlot;

void USPMakingPotionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// ASPMakePotion* PotionMaker = Cast<ASPMakePotion>(GetWorld()->SpawnActor(ASPMakePotion::StaticClass()));
	// if (PotionMaker)
	// {
	// 	// 델리게이트에 위젯의 함수를 바인딩
	// 	PotionMaker->OnWidgetDelegate.AddDynamic(this, &USPMakingPotionWidget::UpdatePotionWidget);
	// }
}

// void USPMakingPotionWidget::UpdatePotionWidget(bool IsVisible)
// {
// 	if(IsVisible)
// 		this->SetVisibility(ESlateVisibility::Visible);
// 	else
// 	{
// 		this->SetVisibility(ESlateVisibility::Hidden);
// 	}
// }



// UUserWidget* USPMakingPotionWidget::CreateItemWidget(USPItemBase* Item)
// {
// 	// Item에 대한 UI 위젯 생성
// 	UUserWidget* ItemWidget = CreateWidget<UUserWidget>(this, USPMakingPotionWidget);
// 	// 위젯에 아이템 데이터 설정하는 코드 추가
// 	return ItemWidget;
// }