// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LobbyWidget.h"

bool ULobbyWidget::ValidateID(FString ID)
{
    // 간단한 검증 로직, 예를 들어 길이 체크
    return ID.Len() > 0;
}