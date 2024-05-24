// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPLobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API ASPLobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

	ASPLobbyPlayerController();
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD )
	TSubclassOf<class USPSelectWidget> LobbyWidgetClass ;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class USPSelectWidget> LobbyWidget;

public:
	UPROPERTY(Replicated)
	uint32 ReadyCount;
	
	UFUNCTION(Server,Unreliable)
	void ServerTEST();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
