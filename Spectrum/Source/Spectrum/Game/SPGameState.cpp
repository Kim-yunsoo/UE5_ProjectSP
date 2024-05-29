// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SPGameState.h"

#include "SpectrumLog.h"
#include "Net/UnrealNetwork.h"
#include "Potion/SPPickup.h"

class ASPScoreTrigger;

ASPGameState::ASPGameState()
{
	GreenScore = 0;
	OrangeScore = 0;
	PurpleScore = 0;
	ReadyCount= 0;

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
		if(HasAuthority())
		{
		++GreenScore;
		On_RapGreenScore();
		}
	}
	else if (MyColor == ColorType::Orange)
	{
		if(HasAuthority())
		{
			++OrangeScore;
			On_RapOrangeScore();
		}
	}
	else if (MyColor == ColorType::Purple)
	{
		if(HasAuthority())
		{
			++PurpleScore;
			On_RapPurpleScore();
		}
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
			if(RemainingTime == SpectrumPotionSpawnTime) //현재 3분이라면? 
			{
				SpectrumPotionSpawn(); //물약 스폰 
			}
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

void ASPGameState::Ready()
{
	++ReadyCount;
	if(ReadyCount==2)
	{
		GetWorld()->ServerTravel(TEXT("/Game/Spectrum/Room/Map/Building?listen"));
	}
}
void ASPGameState::MoveToInGame()
{
	GetWorld()->ServerTravel(TEXT("/Game/Spectrum/Room/Map/Building?listen"));
}

void ASPGameState::SpectrumPotionSpawn()
{
	//GetWorld()->SpawnActorDeferred<ASPPickup>()
}

void ASPGameState::ServerRPC_Implementation()
{
	//SP_LOG(LogSPNetwork,Log,TEXT("In Travel22!!"));
	GetWorld()->ServerTravel(TEXT("/Game/Spectrum/Room/Map/Building?listen"));

}


void ASPGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPGameState, GreenScore);
	DOREPLIFETIME(ASPGameState, OrangeScore);
	DOREPLIFETIME(ASPGameState, PurpleScore);
	DOREPLIFETIME(ASPGameState, RemainingTime);
	DOREPLIFETIME(ASPGameState, ReadyCount);
}


