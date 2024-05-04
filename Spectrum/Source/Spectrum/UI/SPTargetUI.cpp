// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPTargetUI.h"
#include "Interface/SPDamageInterface.h"
#include "Components/Image.h"
#include "Interface/SPCharacterHUDInterface.h"
// #include "Character/SP"

void USPTargetUI::UpdateTargetUI(bool IsAim)
{
	if(IsAim)
		this->SetVisibility(ESlateVisibility::Visible);
	else
	{
		this->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USPTargetUI::NativeConstruct() //위젯에 관련된 UI 요소들이 모두 준비 완료 되면
{
	Super::NativeConstruct();
	
	TargetUIImage = Cast<UImage>(GetWidgetFromName(TEXT("Target")));
	ensure(TargetUIImage);
	this->SetVisibility(ESlateVisibility::Hidden);


}
