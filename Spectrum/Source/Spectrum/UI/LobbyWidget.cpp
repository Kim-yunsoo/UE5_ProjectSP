// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LobbyWidget.h"

bool ULobbyWidget::ValidateID(FString ID)
{
    // ������ ���� ����, ���� ��� ���� üũ
    return ID.Len() > 0;
}