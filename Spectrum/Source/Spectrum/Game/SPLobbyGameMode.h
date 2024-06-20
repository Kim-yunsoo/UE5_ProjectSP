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
public:
	ASPLobbyGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated)
	uint32 ReadyCount;
	
public:
	void Ready();
	
};
