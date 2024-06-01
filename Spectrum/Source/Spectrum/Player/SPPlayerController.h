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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	USPHUDWidget* GetSPHUDWidget() const;


	UFUNCTION(Client,Unreliable)
	void ClientRPCSpawnUI(const int32 Index);

	void OnMathStateSet(FName State); // 매치 상태를 알려주는 함수를 만든다. 

	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;

	UFUNCTION()
	void OnRep_MatchState();

};