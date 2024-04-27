// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPTargetUI.h"
#include "Interface/SPDamageInterface.h"
#include "Components/Image.h"
#include "Interface/SPCharacterHUDInterface.h"
// #include "Character/SP"

void USPTargetUI::UpdateTargetUI(bool IsAim)
{
	UE_LOG(LogTemp, Log, TEXT("UpdateTargetUI"));
	if(IsAim)
		this->SetVisibility(ESlateVisibility::Visible);
	else
	{
		this->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USPTargetUI::NativeConstruct() //������ ���õ� UI ��ҵ��� ��� �غ� �Ϸ� �Ǹ�
{
	Super::NativeConstruct();
	
	TargetUIImage = Cast<UImage>(GetWidgetFromName(TEXT("Target")));
	ensure(TargetUIImage);
	this->SetVisibility(ESlateVisibility::Hidden);

	ISPCharacterHUDInterface* CharacterWidget = Cast<ISPCharacterHUDInterface>(GetOwningPlayerPawn());
	if(CharacterWidget)
	{
		CharacterWidget->SetupTargetWidget(this);
	}
}
