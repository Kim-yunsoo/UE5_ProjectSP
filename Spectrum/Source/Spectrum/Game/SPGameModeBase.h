// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameMode.h"
#include "SPGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class SPECTRUM_API ASPGameModeBase : public AGameMode
{
	GENERATED_BODY()

public:
	ASPGameModeBase();

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override; //���⼭ Ÿ�̸� �����Ѵ�.
	virtual void DefaultGameTimer(); //Ÿ�̸ӷ� ����� �Լ� 
	FTimerHandle GameTimerHandle;

private:
	TSubclassOf<UUserWidget> SPLobbyWidgetClass;
	class USPLobbyWidget* LobbyWidget;


};
