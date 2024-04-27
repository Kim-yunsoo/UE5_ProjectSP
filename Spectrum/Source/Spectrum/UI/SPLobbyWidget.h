// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPLobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API USPLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "ID Verification")
	void ValidateID(const FString& ID);
	
};
