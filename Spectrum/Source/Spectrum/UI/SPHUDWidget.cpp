// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPHUDWidget.h"
#include "Interface/SPCharacterHUDInterface.h"
#include "UI/SPTargetUI.h"

USPHUDWidget::USPHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USPHUDWidget::UpdateTargetUI(bool Iswidget)
{
	if (TargetUI)
	{
		TargetUI->SetVisibility(Iswidget ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		UE_LOG(LogTemp, Log, TEXT("UpdateTargetUI %d"), Iswidget);
	}

}

void USPHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TargetUI = Cast<USPTargetUI>(GetWidgetFromName(TEXT("TargetUI")));
	ensure(TargetUI);

	ISPCharacterHUDInterface* HUDPawn = Cast<ISPCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->CheckTargetUI(this);
	}

}
	

void USPHUDWidget::BindToPlayer(ASPCharacterPlayer* Player)
{
	if (Player)
	{
		// Delegate�� �Լ� ���ε�
		//Player->OnAimingChanged.AddUObject(this, &USPHUDWidget::USPHUDWidget);
	}
}
