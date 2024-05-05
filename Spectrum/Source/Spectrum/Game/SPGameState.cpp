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
		GetWorldTimerManager().SetTimer(GameTimerHandle, this, &ASPGameState::DefaultGameTimer,
		                                GetWorldSettings()->GetEffectiveTimeDilation(), true);
}

void ASPGameState::AddScore(const ColorType& MyColor)
{
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

	}
}

void ASPGameState::DefaultGameTimer()
{

	if(HasAuthority())
	{
		if (RemainingTime > 0)
		{
			RemainingTime--;
			OnRapTime();
		}
	}
}

void ASPGameState::OnRapTime()
{
	OnTime.Broadcast(RemainingTime);
}


void ASPGameState::MultiRPC_Implementation(const ColorType& Mycolor, const int Score)
{
	OnScore.Broadcast(Mycolor, Score);
}


void ASPGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPGameState, GreenScore);
	DOREPLIFETIME(ASPGameState, OrangeScore);
	DOREPLIFETIME(ASPGameState, PurpleScore);
	DOREPLIFETIME(ASPGameState, RemainingTime);
}
