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
}

void ASPGameState::BeginPlay()
{
	Super::BeginPlay();
}

void ASPGameState::AddScore(const ColorType& MyColor)
{
	SP_LOG(LogSPNetwork,Log,TEXT("AddScore"));
	if (MyColor == ColorType::Green)
	{
		++GreenScore;
		MultiRPC(MyColor,GreenScore);
		// OnRep_AddSocreEnvet(MyColor, GreenScore); //�� �̺�Ʈ�� ���������� �θ��ϱ� ������ �����. 
	}
	else if (MyColor == ColorType::Orange)
	{
		++OrangeScore;

		MultiRPC(MyColor,OrangeScore);
		// OnRep_AddSocreEnvet(MyColor, OrangeScore);
	}
	else if (MyColor == ColorType::Purple)
	{
		++PurpleScore;
		MultiRPC(MyColor,PurpleScore);

		// OnRep_AddSocreEnvet(MyColor, PurpleScore);
	}
}

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
