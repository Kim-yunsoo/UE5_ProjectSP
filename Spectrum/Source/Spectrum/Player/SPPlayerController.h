// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPPlayerController.generated.h"

/**
 *
 */
UCLASS()
class SPECTRUM_API ASPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASPPlayerController();

protected:
	virtual void BeginPlay() override;

 //HUD Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class USPHUDWidget> SPHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class USPHUDWidget> SPHUDWidget;

public:

};
