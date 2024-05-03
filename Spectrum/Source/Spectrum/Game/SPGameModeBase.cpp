// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SPGameModeBase.h"

#include "SpectrumLog.h"
#include "SPGameState.h"
#include "UI/SPLobbyWidget.h"
#include "GameFramework/GameStateBase.h"

ASPGameModeBase::ASPGameModeBase()
{
	GameStateClass=ASPGameState::StaticClass();
	
	//static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));

	//if (ThirdPersonClassRef.Class)
	//{
	//	DefaultPawnClass = ThirdPersonClassRef.Class;
	//}

	//static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/Spectrum.SPCharacterPlayer"));
	//if (DefaultPawnClassRef.Class)
	//{
	//	DefaultPawnClass = DefaultPawnClassRef.Class;
	//}

	//static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/Spectrum.SPPlayerController"));
	//if (PlayerControllerClassRef.Class) {
	//	PlayerControllerClass = PlayerControllerClassRef.Class;
	//}

	//static ConstructorHelpers::FClassFinder< UUserWidget> LobbyWidgetClassRef(TEXT("/Game/Spectrum/StartUI/StartMenuWidget.StartMenuWidget"));
	//if (LobbyWidgetClassRef.Class)
	//{
	//	SPLobbyWidgetClass = LobbyWidgetClassRef.Class;
	//}	
}

void ASPGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//if (SPLobbyWidgetClass)
	//{
	//	LobbyWidget = CreateWidget<USPLobbyWidget>(GetWorld(), SPLobbyWidgetClass);
	//	if (LobbyWidget)
	//	{
	//		LobbyWidget->AddToViewport();
	//	}
	//}
	
}

void ASPGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	GetWorldTimerManager().SetTimer(GameTimerHandle, this, &ASPGameModeBase::DefaultGameTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);

}

void ASPGameModeBase::DefaultGameTimer()
{
	ASPGameState* const SPGameState = Cast<ASPGameState>(GameState);
	if(SPGameState && SPGameState->RemainingTime>0)
	{
		SPGameState->RemainingTime--;
		//어떤 이벤트를 날려야하는데 .
		ClientRPC();
	}
}

void ASPGameModeBase::ClientRPC_Implementation()
{
	SP_LOG(LogSPNetwork,Log,TEXT("ModeTImeTest"));
}
