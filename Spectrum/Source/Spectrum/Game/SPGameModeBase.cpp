// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SPGameModeBase.h"

#include "Game/SPGameState.h"
#include "UI/SPLobbyWidget.h"
#include "GameFramework/GameStateBase.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "../Spectrum/Player/SPPlayerController.h"

//extern Protocol::PlayerType player_type;
extern Protocol::PlayerType school_num_type;
extern int32 NumPlayers;
extern std::array<Protocol::PlayerType, 3> school_type;
//extern FString IPAddress;

ASPGameModeBase::ASPGameModeBase()
{
   
   GameStateClass=ASPGameState::StaticClass();
   bUseSeamlessTravel=true;
   
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

}

//void ASPGameModeBase::goServer()
//{
//    UWorld* World = GetWorld();
//    World->ServerTravel("192.168.41.20");
//
//}


void ASPGameModeBase::PostLogin(APlayerController* NewPlayer)
{
      Super::PostLogin(NewPlayer);

      GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("0000000000000000")));


      // 정보 받아서 폰 설정 
      auto CastPlayer = Cast<ASPPlayerController>(NewPlayer);
      if (CastPlayer)
      {
         //USpectrumGameInstance* pawn_PlayerType = Cast<USpectrumGameInstance>(NewPlayer->PlayerState);

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
}
