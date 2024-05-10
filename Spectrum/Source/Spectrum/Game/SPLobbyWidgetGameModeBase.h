// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SPLobbyWidgetGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API ASPLobbyWidgetGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	ASPLobbyWidgetGameModeBase();

public:
	bool bAlreadyTraveled;
	void goServer();


public:
	virtual void PostInitializeComponents() override; //여기서 타이머 가동한다. 
	virtual void DefaultGameTimer(); //타이머로 사용할 함수
	
};
