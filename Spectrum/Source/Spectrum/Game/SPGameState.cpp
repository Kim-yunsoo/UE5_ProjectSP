// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SPGameState.h"
#include "Net/UnrealNetwork.h"

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
		On_RapGreenScore();
	}
	else if (MyColor == ColorType::Orange)
	{
		++OrangeScore;
		On_RapOrangeScore();
	}
	else if (MyColor == ColorType::Purple)
	{
		++PurpleScore;
		On_RapPurpleScore();
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

void ASPGameState::On_RapGreenScore()
{
	OnScore.Broadcast(ColorType::Green, GreenScore);
}

void ASPGameState::On_RapOrangeScore()
{
	OnScore.Broadcast(ColorType::Orange, OrangeScore);
}

void ASPGameState::On_RapPurpleScore()
{
	OnScore.Broadcast(ColorType::Purple, PurpleScore);
}

void ASPGameState::OnRapTime()
{
	OnTime.Broadcast(RemainingTime);
}





void ASPGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPGameState, GreenScore);
	DOREPLIFETIME(ASPGameState, OrangeScore);
	DOREPLIFETIME(ASPGameState, PurpleScore);
	DOREPLIFETIME(ASPGameState, RemainingTime);
}
