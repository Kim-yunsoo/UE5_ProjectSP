// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SPLobbyPlayerController.h"

#include "SpectrumLog.h"
#include "SPLobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"
//#include "UI/SPSelectWidget.h"

ASPLobbyPlayerController::ASPLobbyPlayerController()
{
	//static ConstructorHelpers::FClassFinder<USPSelectWidget>WidgetRef(TEXT("/Game/SpectrumStart/UI/WBP_SelectWidget.WBP_SelectWidget_C"));

	// if(WidgetRef.Class)
	// {
	// 	LobbyWidgetClass = WidgetRef.Class;
	// }
	ReadyCount=0;

}

void ASPLobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// 게임 모드 설정
	FInputModeGameAndUI GameInputMode;
	SetInputMode(GameInputMode);
	SetShowMouseCursor(true);

	// if (IsLocalPlayerController())
	// {
	// 	LobbyWidget = CreateWidget<USPSelectWidget>(this, LobbyWidgetClass);
	// 	if (LobbyWidget)
	// 	{
	// 		LobbyWidget->AddToViewport();
	// 		LobbyWidget->SetVisibility(ESlateVisibility::Visible);
	// 	}
	// }
	//
	// if (IsLocalPlayerController())
	// {
	// 	LobbyHUD = CreateWidget<USPLobbyHUDWidget>(this, LobbyHUDClass);
	// 	if (LobbyHUD)
	// 	{
	// 		LobbyHUD->AddToViewport();
	// 		LobbyHUD->SetVisibility(ESlateVisibility::Visible);
	// 	}
	// }
}

void ASPLobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASPLobbyPlayerController, ReadyCount);

}

void ASPLobbyPlayerController::ServerTEST_Implementation()
{
	SP_LOG(LogSPNetwork,Log,TEXT("Come in!!"));
	UE_LOG(LogTemp,Log,TEXT("%s"),*(GetWorld()->GetGameState())->GetName())
	UE_LOG(LogTemp,Log,TEXT("%s"),*(GetWorld()->GetAuthGameMode())->GetName())
	ASPLobbyGameMode* LobbyMode = Cast<ASPLobbyGameMode>(GetWorld()->GetAuthGameMode());
	if(LobbyMode)
	{
		LobbyMode->Ready();
	}
	//Cast<GameState>()GetWorld()->GetGameState();
	//++ReadyCount;
	// SP_LOG(LogSPNetwork,Log,TEXT("ReadyCount -----  %d"), ReadyCount);
	//
	// if(ReadyCount==2)
	// {
	// 	SP_LOG(LogSPNetwork,Log,TEXT("ReadyCount -  %d"), ReadyCount);
	//
	// 	GetWorld()->ServerTravel(TEXT("/Game/Spectrum/Room/Map/Building?listen"));
	// }
}

