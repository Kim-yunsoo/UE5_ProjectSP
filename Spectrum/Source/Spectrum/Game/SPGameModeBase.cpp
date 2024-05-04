// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SPGameModeBase.h"
#include "UI/SPLobbyWidget.h"
#include "GameFramework/GameStateBase.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "../Spectrum/Player/SPPlayerController.h"
//extern Protocol::PlayerType player_type;
extern Protocol::PlayerType school_num_type;
extern bool okok;
bool bAlreadyTraveled = false;
extern int32 NumPlayers;
extern std::array<Protocol::PlayerType, 3> school_type;

ASPGameModeBase::ASPGameModeBase()
{
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
	//bUseSeamlessTravel = true; // Seamless Travel 활성화
}

bool ASPGameModeBase::TryChangePawn(APlayerController* pCon, FVector location, TSubclassOf<APawn> PAWN_C)
{
	if (alreadyChange.Contains(pCon)) return false;

	auto newPawn = UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), PAWN_C, nullptr, location, FRotator::ZeroRotator);
	if (!newPawn) return false;

	alreadyChange.Add(pCon, true);
	pCon->Possess(newPawn);
	return true;
}

void ASPGameModeBase::TryDestroyOldpawn_Implementation(APawn* pawn)
{
	if (pawn->IsValidLowLevel())
		pawn->Destroy();
}

void ASPGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//TravelToNextLevel();
	//if (SPLobbyWidgetClass)
	//{
	//	LobbyWidget = CreateWidget<USPLobbyWidget>(GetWorld(), SPLobbyWidgetClass);
	//	if (LobbyWidget)
	//	{
	//		LobbyWidget->AddToViewport();
	//	}
	//}
}

//void ASPGameModeBase::PostInitializeComponents()
//{
//	Super::PostInitializeComponents();
//	FTimerHandle GameTimerHandle;
//	GetWorldTimerManager().SetTimer(GameTimerHandle, this, &ASPGameModeBase::DefaultGameTimer, 1.0f, false);
//}
//
//void ASPGameModeBase::DefaultGameTimer()
//{
//	if (bAlreadyTraveled == false) {
//		bAlreadyTraveled = true;
//		UE_LOG(LogTemp, Log, TEXT("Default"));
//		//GetWorld()->ServerTravel(TEXT("/Game/Spectrum/Room/Map/Building?listen")); // 다른 월드로 이동한다.
//		//okok = true;
//	}
//}

//void ASPGameModeBase::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//	//if (okok) 
//	//GetWorld()->ServerTravel(TEXT("/Game/Spectrum/Room/Map/Building?listen")); // 다른 월드로 이동
//
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Tick")));
//}

void ASPGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	//_sleep(3000);
	//okok = true;

	//if (okok) {

		Super::PostLogin(NewPlayer);

		// 정보 받아서 폰 설정 
		auto CastPlayer = Cast<ASPPlayerController>(NewPlayer);
		if (CastPlayer)
		{
			USpectrumGameInstance* pawn_PlayerType = Cast<USpectrumGameInstance>(NewPlayer->PlayerState);

			//pawn_PlayerType->PlayerType = 0;

			// 정보에 따라 폰 설정
			if (mynum < 3)
			{
				school_num_type = school_type[mynum];
				mynum++;
			}

			switch (school_num_type)
			{
			case Protocol::PLAYER_TYPE_GREEN_MAN:
				CastPlayer->ChangePawnName(TEXT("/Game/Spectrum/BluePrint/BP_SPCharacterMan2"));
				break;
			case Protocol::PLAYER_TYPE_GREEN_WOMAN:
				CastPlayer->ChangePawnName(TEXT("/Game/Spectrum/BluePrint/BP_SPCharaterPlayer_W2"));
				break;
			case Protocol::PLAYER_TYPE_PURPLE_MAN:
				CastPlayer->ChangePawnName(TEXT("/Game/Spectrum/BluePrint/BP_SPCharacterMan1"));
				break;
			case Protocol::PLAYER_TYPE_PURPLE_WOMAN:
				CastPlayer->ChangePawnName(TEXT("/Game/Spectrum/BluePrint/BP_SPCharaterPlayer_W1"));
				break;
			case Protocol::PLAYER_TYPE_ORANGE_MAN:
				CastPlayer->ChangePawnName(TEXT("/Game/Spectrum/BluePrint/BP_SPCharacterMan3"));
				break;
			case Protocol::PLAYER_TYPE_ORANGE_WOMAN:
				CastPlayer->ChangePawnName(TEXT("/Game/Spectrum/BluePrint/BP_SPCharaterPlayer_W3"));
				break;
			}

		}

		//UWorld* World = GetWorld();
		//if (!ensure(World != nullptr)) return;

		//World->ServerTravel("Building?listen");
		//}
	


}

void ASPGameModeBase::TravelToNextLevel()
{
	UWorld* World = GetWorld();
if (!ensure(World != nullptr)) return;

World->ServerTravel("Building?listen");
}