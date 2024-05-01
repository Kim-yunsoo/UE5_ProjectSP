// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPDragWidget.generated.h"

class ASPCharacterPlayer;
/**
 * 
 */
UCLASS()
class SPECTRUM_API USPDragWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;

	TObjectPtr<ASPCharacterPlayer> MyCharacter;
};
