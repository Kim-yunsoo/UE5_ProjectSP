// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SPGameModeBase.h"

#include "SpectrumLog.h"
#include "SPPlayerState.h"
#include "Game/SPGameState.h"
#include "UI/SPLobbyWidget.h"
#include "GameFramework/GameStateBase.h"
#include "Character/SPCharacterPlayer.h"
#include "Player/SPPlayerController.h"

//extern Protocol::PlayerType player_type;
extern Protocol::PlayerType school_num_type;
extern int32 NumPlayers;
extern std::array<Protocol::PlayerType, 3> school_type;
//extern FString IPAddress;
//extern PlayerInfomation MyPlayerInfo;

ASPGameModeBase::ASPGameModeBase()
{
	GameStateClass = ASPGameState::StaticClass();
	PlayerStateClass = ASPPlayerState::StaticClass();
	PlayerControllerClass= ASPPlayerController::StaticClass();
	bUseSeamlessTravel = true;
	// 디폴트 폰 클래스 /Game/Spectrum/BluePrint/BP_SPCharacterMan2로 설정
}

// bool ASPGameModeBase::TryChangePawn(APlayerController* pCon, FVector location, TSubclassOf<APawn> PAWN_C)
// {
// 	if (alreadyChange.Contains(pCon)) return false;
//
// 	auto newPawn = UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), PAWN_C, nullptr, location,
// 	                                                           FRotator::ZeroRotator);
// 	if (!newPawn) return false;
//
// 	alreadyChange.Add(pCon, true);
// 	pCon->Possess(newPawn);
// 	return true;
// }
//
// void ASPGameModeBase::TryDestroyOldpawn_Implementation(APawn* pawn)
// {
// 	if (pawn->IsValidLowLevel())
// 		pawn->Destroy();
// }

void ASPGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASPGameModeBase::HandleSeamlessTravelPlayer(AController*& C)
{
	Super::HandleSeamlessTravelPlayer(C);

	if(!C)
	{
		return;
	}

	if(ASPPlayerController* MyController = Cast<ASPPlayerController>(C))
	{
		if(ASPPlayerState* PlayerState = MyController->GetPlayerState<ASPPlayerState>())
		{
			if(PlayerState)
			{
				SpawnPlayerCharacter(MyController, PlayerState->Color,PlayerState->Gender);
			}
		}
	}
}

void ASPGameModeBase::SpawnPlayerCharacter(APlayerController* MyController, const ColorType& MyColor,
	const GenderType& MyGender)
{
	FString SpawnPath;
	if(MyColor ==ColorType::Green && MyGender == GenderType::Man)
	{
		SpawnPath= TEXT("/Game/Spectrum/BluePrint/BP_SPCharacterMan2.BP_SPCharacterMan2_C");
	}
	else if(MyColor ==ColorType::Orange && MyGender == GenderType::Man)
	{
		SpawnPath = TEXT("/Game/Spectrum/BluePrint/BP_SPCharacterMan3.BP_SPCharacterMan3_C");
	}
	else if(MyColor ==ColorType::Purple && MyGender == GenderType::Man)
	{
		SpawnPath =  TEXT("/Game/Spectrum/BluePrint/BP_SPCharacterMan1.BP_SPCharacterMan1_C");
	}
	
	else if(MyColor ==ColorType::Green && MyGender == GenderType::Woman)
	{
		SpawnPath =  TEXT("/Game/Spectrum/BluePrint/BP_SPCharaterPlayer_W2.BP_SPCharaterPlayer_W2_C");
	}
	else if(MyColor ==ColorType::Orange && MyGender == GenderType::Woman)
	{
		SpawnPath = TEXT("/Game/Spectrum/BluePrint/BP_SPCharaterPlayer_W3.BP_SPCharaterPlayer_W3_C");
	}
	else if(MyColor ==ColorType::Purple && MyGender == GenderType::Woman)
	{
		SpawnPath = TEXT("/Game/Spectrum/BluePrint/BP_SPCharaterPlayer_W1.BP_SPCharaterPlayer_W1_C");
	}
	
	UClass* DesiredPawnClass = StaticLoadClass(UObject::StaticClass(), nullptr, *SpawnPath);

	if(DesiredPawnClass)
	{
		APawn* OldPawn = MyController->GetPawn();
		if(OldPawn)
		{
			OldPawn->Destroy();
		}

		FVector SpawnLocation = FVector(2547.0f, 247.0f, 2894.0f);
		// 	// 새 폰을 스폰하고 플레이어에게 할당
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = MyController;
		APawn* NewPawn = GetWorld()->SpawnActor<APawn>(DesiredPawnClass, SpawnLocation, FRotator::ZeroRotator,
		 	                                               SpawnParams);
		MyController->Possess(NewPawn);
	}
}
void ASPGameModeBase::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();
}

void ASPGameModeBase::SendMessagesToEveryOne(const FString& Sender, const FString& Message)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController)
		{
			//APawn* Pawn =PlayerController->GetPawn();
			//체크
			ASPCharacterPlayer* MyPlayer = Cast<ASPCharacterPlayer>(PlayerController->GetPawn());
			if (MyPlayer)
			{
				MyPlayer->ClientRPCAddMessageToChat(Sender, Message);
			}
		}
	}
}

