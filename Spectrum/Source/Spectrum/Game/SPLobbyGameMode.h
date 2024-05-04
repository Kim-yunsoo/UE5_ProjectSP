// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SPLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API ASPLobbyGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	virtual void PostInitializeComponents() override; //���⼭ Ÿ�̸� �����Ѵ�. 
	virtual void DefaultGameTimer(); //Ÿ�̸ӷ� ����� �Լ� 
	FTimerHandle GameTimerHandle;
	
};
