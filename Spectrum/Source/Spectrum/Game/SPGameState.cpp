// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SPGameState.h"
#include "EngineUtils.h"
#include "SpectrumLog.h"
#include "Net/UnrealNetwork.h"
#include "Object/Trigger/SPScoreTrigger.h"

class ASPScoreTrigger;

ASPGameState::ASPGameState()
{
	GreenScore = 0;
	OrangeScore = 0;
	PurpleScore = 0;
	RemainingTime = MatchPlayTime;
}

void ASPGameState::BeginPlay()
{
	Super::BeginPlay();
}

void ASPGameState::AddScore(const ColorType& MyColor)
{
	SP_LOG(LogSPNetwork, Log, TEXT("AddScore"));
	if (MyColor == ColorType::Green)
	{
		++GreenScore;
		MultiRPC(MyColor, GreenScore);
		// OnRep_AddSocreEnvet(MyColor, GreenScore); //이 이벤트를 서버에서만 부르니까 문제가 생긴다. 
	}
	else if (MyColor == ColorType::Orange)
	{
		++OrangeScore;

		MultiRPC(MyColor, OrangeScore);
		// OnRep_AddSocreEnvet(MyColor, OrangeScore);
	}
	else if (MyColor == ColorType::Purple)
	{
		++PurpleScore;
		MultiRPC(MyColor, PurpleScore);

		// OnRep_AddSocreEnvet(MyColor, PurpleScore);
	}
}

// void ASPGameState::PostInitializeComponents()
// {
// 	Super::PostInitializeComponents();
// 	// GetWorldTimerManager().SetTimer(GameTimerHandle, this, &ASPGameState::DefaultGameTimer,
// 	//                                 GetWorldSettings()->GetEffectiveTimeDilation(), true);
// }

// void ASPGameState::DefaultGameTimer()
// {
// 		// ASPGameState* const SPGameState = Cast<ASPGameState>(GameState);
// 		// UE_LOG(LogTemp,Log,TEXT("TEST1"));
// 	
// 		 if(RemainingTime>0)
// 		{
// 			RemainingTime--;
// 			//어떤 이벤트를 날려야하는데 .
// 			// UE_LOG(LogTemp,Log,TEXT("TEST2"));
// 			// ClientRPC();
// 		}
// }

// void ASPGameState::ClientRPC_Implementation()
// {
// 	SP_LOG(LogSPNetwork,Log,TEXT("ModeTImeTest"));
// }

void ASPGameState::MultiRPC_Implementation(const ColorType& Mycolor, const int Score)
{
	OnScore.Broadcast(Mycolor, Score);
}

// void ASPGameState::OnRep_AddSocreEnvet()
// {
// 	OnScore.Broadcast(MyColor, Score);
// }

void ASPGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPGameState, GreenScore);
	DOREPLIFETIME(ASPGameState, OrangeScore);
	DOREPLIFETIME(ASPGameState, PurpleScore);

	DOREPLIFETIME(ASPGameState, RemainingTime);
}
