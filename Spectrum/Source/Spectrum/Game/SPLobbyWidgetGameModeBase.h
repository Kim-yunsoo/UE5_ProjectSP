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
	virtual void PostInitializeComponents() override; //���⼭ Ÿ�̸� �����Ѵ�. 
	virtual void DefaultGameTimer(); //Ÿ�̸ӷ� ����� �Լ�
	
};
