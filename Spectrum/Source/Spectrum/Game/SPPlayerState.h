// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPPlayerState.generated.h"

/**
 * 
 */
enum class GenderType : uint8;
enum class ColorType : uint8;

UCLASS()
class SPECTRUM_API ASPPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	ASPPlayerState();
	

public:
	virtual void CopyProperties(class APlayerState* PlayerState) override;
	UPROPERTY()
	ColorType Color;
	UPROPERTY()
	GenderType Gender;

	UPROPERTY()
	FString NickName;

	UPROPERTY()
	uint32 Rank;

	UFUNCTION(Server,Unreliable)
	void SetPlayerColor(ColorType MyColor);
	UFUNCTION(Server,Unreliable)
	void SetPlayerGender(GenderType MyGender);
	UFUNCTION(Server,Unreliable)
	void SetPlayerNickName(const FString& Text);	
};
