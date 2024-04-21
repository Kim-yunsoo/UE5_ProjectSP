// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPWidgetComponent.h"
#include "SPUserWidget.h"

void USPWidgetComponent::InitWidget()
{
	Super::InitWidget(); 
	//������ ���� �ν��Ͻ��� ������ ����
	USPUserWidget* SPUserWidget = Cast<USPUserWidget>(GetWidget());
	if (SPUserWidget)
	{
		SPUserWidget->SetOwningActor(GetOwner());
	}
}
