// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPWidgetComponent.h"
#include "SPUserWidget.h"

void USPWidgetComponent::InitWidget()
{
	Super::InitWidget(); 
	//위젯에 대한 인스턴스가 생성된 직후
	USPUserWidget* SPUserWidget = Cast<USPUserWidget>(GetWidget());
	if (SPUserWidget)
	{
		SPUserWidget->SetOwningActor(GetOwner());
	}
}
