// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameMode.h"
#include "SPGameModeBase.generated.h"

class ASPPlayerController;
enum class GenderType : uint8;
enum class ColorType : uint8;
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
	virtual void HandleSeamlessTravelPlayer(AController*& C) override;
	void SpawnPlayerCharacter(APlayerController* MyController,const ColorType& MyColor,const GenderType& MyGender);
	virtual void PostLogin(APlayerController* NewPlayer) override;


private:
	TSubclassOf<UUserWidget> SPLobbyWidgetClass;
public:
	void SendMessagesToEveryOne(const FString& Sender, const FString& Message);
	virtual void PostSeamlessTravel() override;

};