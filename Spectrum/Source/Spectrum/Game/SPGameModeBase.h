// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "UI/SPLobbyWidget.h"
#include "SPGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class SPECTRUM_API ASPGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASPGameModeBase();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool TryChangePawn(APlayerController* pCon, FVector location, TSubclassOf<APawn> PAWN_C);

	UFUNCTION(NetMulticast, Reliable)
	void TryDestroyOldpawn(APawn* pawn);

	virtual void BeginPlay() override;

	void TravelToNextLevel();

	//virtual void Tick(float DeltaTime) override;
	//void PostInitializeComponents() override;
protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	TSubclassOf<UUserWidget> SPLobbyWidgetClass;
	class USPLobbyWidget* LobbyWidget;

	TMap<APlayerController*, bool> alreadyChange;
	//TArray<APlayerController*> JoinPlayers;

	//virtual void PostInitializeComponents() override; //여기서 타이머 가동한다. 
	//virtual void DefaultGameTimer(); //타이머로 사용할 함수

};
