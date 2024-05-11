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
//extern PlayerInfomation MyPlayerInfo;

ASPGameModeBase::ASPGameModeBase()
{
   
   GameStateClass=ASPGameState::StaticClass();
   bUseSeamlessTravel=true;
   
   // 디폴트 폰 클래스 /Game/Spectrum/BluePrint/BP_SPCharacterMan2로 설정

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

      //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("0000000000000000")));
	  FString BlueprintPath = TEXT("/Game/Spectrum/BluePrint/BP_SPCharaterPlayer_W3.BP_SPCharaterPlayer_W3_C"); // 임시 경로

      // 정보에 따라 폰 설정
      if (mynum < 3)
      {
          school_num_type = school_type[mynum];
          //mynum++;
      }

      switch (school_num_type)
      {
      case Protocol::PLAYER_TYPE_GREEN_MAN:
          BlueprintPath = TEXT("/Game/Spectrum/BluePrint/BP_SPCharacterMan1.BP_SPCharacterMan1_C"); 
          mynum++;
          break;
      case Protocol::PLAYER_TYPE_GREEN_WOMAN:
          BlueprintPath = TEXT("/Game/Spectrum/BluePrint/BP_SPCharaterPlayer_W2.BP_SPCharaterPlayer_W2_C");
          mynum++;
          break;
      case Protocol::PLAYER_TYPE_PURPLE_MAN:
          BlueprintPath = TEXT("/Game/Spectrum/BluePrint/BP_SPCharacterMan2.BP_SPCharacterMan2_C"); 
          mynum++;
          break;
      case Protocol::PLAYER_TYPE_PURPLE_WOMAN:
          BlueprintPath = TEXT("/Game/Spectrum/BluePrint/BP_SPCharaterPlayer_W1.BP_SPCharaterPlayer_W1_C");
          mynum++;
          break;
      case Protocol::PLAYER_TYPE_ORANGE_MAN:
          BlueprintPath = TEXT("/Game/Spectrum/BluePrint/BP_SPCharacterMan3.BP_SPCharacterMan3_C"); 
          mynum++;
          break;
      case Protocol::PLAYER_TYPE_ORANGE_WOMAN:
          BlueprintPath = TEXT("/Game/Spectrum/BluePrint/BP_SPCharaterPlayer_W3.BP_SPCharaterPlayer_W3_C");
          mynum++;
          break;
      }

      // 블루프린트 클래스를 로드
      UClass* DesiredPawnClass = StaticLoadClass(UObject::StaticClass(), nullptr, *BlueprintPath);

      if (DesiredPawnClass)
      {
          // 기존 폰이 있으면 제거
          if (APawn* OldPawn = NewPlayer->GetPawn())
          {
              OldPawn->Destroy();
          }

          FVector SpawnLocation = FVector(1970.0f, 140.0f, 5192.0f);
          // 새 폰을 스폰하고 플레이어에게 할당
          FActorSpawnParameters SpawnParams;
          SpawnParams.Owner = NewPlayer;
          APawn* NewPawn = GetWorld()->SpawnActor<APawn>(DesiredPawnClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
          NewPlayer->Possess(NewPawn);
      }

}
