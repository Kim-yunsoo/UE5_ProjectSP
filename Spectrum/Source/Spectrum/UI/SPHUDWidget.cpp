// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPHUDWidget.h"
#include "UI/SPTargetUI.h"
#include "UI/SPInventoryWidget.h"

USPHUDWidget::USPHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}



void USPHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TargetUI = Cast<USPTargetUI>(GetWidgetFromName(TEXT("WBTargetUI")));
	ensure(TargetUI);

	Inventory = Cast<USPInventoryWidget>(GetWidgetFromName(TEXT("Inventory")));
	ensure(TargetUI);
	
}
	


