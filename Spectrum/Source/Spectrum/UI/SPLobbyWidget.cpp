// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPLobbyWidget.h"

void USPLobbyWidget::ValidateID(const FString& ID)
{
    // 간단한 예시: 아이디가 비어 있지 않아야 함
    if( !ID.IsEmpty())
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ID:%s"), *ID));

}