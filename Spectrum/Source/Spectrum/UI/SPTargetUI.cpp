// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPTargetUI.h"
#include "Components/Image.h"
// #include "Character/SP"



void USPTargetUI::NativeConstruct() //������ ���õ� UI ��ҵ��� ��� �غ� �Ϸ� �Ǹ�
{
	Super::NativeConstruct();
	
	TargetUIImage = Cast<UImage>(GetWidgetFromName(TEXT("Target")));
	ensure(TargetUIImage);
	this->SetVisibility(ESlateVisibility::Hidden);
}


void USPTargetUI::UpdateTargetUI(bool IsAim)
{
	if(IsAim)
		this->SetVisibility(ESlateVisibility::Visible);
	else
	{
		this->SetVisibility(ESlateVisibility::Hidden);
	}
}