// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPLobbyWidget.h"

void USPLobbyWidget::ValidateID(const FString& ID)
{
    // ������ ����: ���̵� ��� ���� �ʾƾ� ��
    if( !ID.IsEmpty())
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ID:%s"), *ID));

}