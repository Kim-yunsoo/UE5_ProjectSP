// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SPPlayerState.h"

ASPPlayerState::ASPPlayerState()
{
}

void ASPPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
	ASPPlayerState* MyPlayerState = Cast<ASPPlayerState>(PlayerState);
	if(MyPlayerState)
	{
		MyPlayerState->Color=Color;
		MyPlayerState->Gender=Gender;
		MyPlayerState->NickName=NickName;
	}
}


void ASPPlayerState::SetPlayerColor_Implementation(ColorType MyColor)
{
	Color= MyColor;
}


void ASPPlayerState::SetPlayerNickName_Implementation(const FString& Text)
{
	SetPlayerName(Text);
}

void ASPPlayerState::SetPlayerGender_Implementation(GenderType MyGender)
{
	Gender=MyGender;
}


