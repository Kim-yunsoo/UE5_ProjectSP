// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Data/SPColorScoreData.h"
#include "SPPlayerController.generated.h"

class USPReturnToMainMenu;
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> Quit;
	
	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class USPHUDWidget> SPHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class USPHUDWidget> SPHUDWidget;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<class USPReturnToMainMenu> EndWidgetClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USPReturnToMainMenu> EndWidget;
public:
	UFUNCTION(Client,Unreliable)
	void ClientRPCReturnToMainMenu(const TArray<FColorScoreData>& ColorScoreDataArray);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	USPHUDWidget* GetSPHUDWidget() const;


	UFUNCTION(Client,Unreliable)
	void ClientRPCSpawnUI(const int32 Index);

	UFUNCTION(Client,Unreliable)
	void ClientAIAlarmUI();

	UFUNCTION(Client,Unreliable)
	void ClientRCPMathState(FName State); // 매치 상태를 알려주는 함수를 만든다. 

	UPROPERTY()
	FName MatchState;

private:

	UPROPERTY()
	TSubclassOf<class UUserWidget> ReturnToMainMenuWidget;

	UPROPERTY()
	TObjectPtr<USPReturnToMainMenu> ReturnToMainMenu;

	uint8 bReturnToMainMenuOpen :1;
	

public:
	//chatting
	UFUNCTION(Server,Unreliable)
	void ServerRPCSendMessage(const FString& Sender, const FString& Message );
	UFUNCTION(Client,Unreliable)
	void ClientRPCAddMessageToChat(const FString& Sender, const FString& Message );
};

