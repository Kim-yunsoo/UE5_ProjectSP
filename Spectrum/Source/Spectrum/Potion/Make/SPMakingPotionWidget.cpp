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
	// 	// ��������Ʈ�� ������ �Լ��� ���ε�
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
// 	// Item�� ���� UI ���� ����
// 	UUserWidget* ItemWidget = CreateWidget<UUserWidget>(this, USPMakingPotionWidget);
// 	// ������ ������ ������ �����ϴ� �ڵ� �߰�
// 	return ItemWidget;
// }