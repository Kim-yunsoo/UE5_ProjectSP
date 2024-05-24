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

	virtual void OnPossess(APawn* aPawn) override;

	// UFUNCTION(BlueprintCallable)
	// void ChangePawnName(FString path);
	// UFUNCTION(Server, Reliable)
	// void CallServerDuetoChangePawn(FVector location, TSubclassOf<APawn> PAWN_C, APawn* OLDpAWN);

protected:
	virtual void BeginPlay() override;

	//HUD Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class USPHUDWidget> SPHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class USPHUDWidget> SPHUDWidget;

public:
	USPHUDWidget* GetSPHUDWidget() const;

	void SetHUDMatchCountdown(float CountdownTime);

};