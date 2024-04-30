// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPLobbyWidget.h"
#include "Spectrum.h"
#include "ClientPacketHandler.h"
#include "Protocol.pb.h"

extern bool issuccess;

USPLobbyWidget::USPLobbyWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	School = -1;
	Gender = -1;
}

void USPLobbyWidget::ValidateID(const FString& ID)
{
    // 아이디 화면에 출력(확인용)
    //if( !ID.IsEmpty())
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ID:%s"), *ID));

	std::string myid = TCHAR_TO_UTF8(*ID);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ID:%s"), myid));

	// 아이디가 DB에 있는지 확인하기 위해 서버로 보냄
	{ // 로그인	패킷 보내기
		Protocol::C_LOGIN Pkt;
		Pkt.set_membership_id(myid);

		SEND_PACKET(Pkt);
	}

}

bool USPLobbyWidget::LoginSuccess()
{
	// 로그인 성공시 true 반환
	if (issuccess)
		return true;
	else
		return false;
}


void USPLobbyWidget::Membership(const FString& ID, const int& SCHOOL, const int& GENDER)
{
	std::string myid = TCHAR_TO_UTF8(*ID);
	Protocol::PlayerType mytype;

	/*PLAYER_TYPE_GREEN_MAN = 00;
	PLAYER_TYPE_GREEN_WOMAN = 01;
	PLAYER_TYPE_PURPLE_MAN = 10;
	PLAYER_TYPE_PURPLE_WOMAN = 11;
	PLAYER_TYPE_ORANGE_MAN = 20;
	PLAYER_TYPE_ORANGE_WOMAN = 21;*/

	switch (SCHOOL)
	{
	case 0:
		if (GENDER == 0)
			mytype = Protocol::PlayerType::PLAYER_TYPE_GREEN_MAN;
		else
			mytype = Protocol::PlayerType::PLAYER_TYPE_GREEN_WOMAN;
		break;
	case 1:
		if (GENDER == 0)
			mytype = Protocol::PlayerType::PLAYER_TYPE_PURPLE_MAN;
		else
			mytype = Protocol::PlayerType::PLAYER_TYPE_PURPLE_WOMAN;
		break;
	case 2:
		if (GENDER == 0)
			mytype = Protocol::PlayerType::PLAYER_TYPE_ORANGE_MAN;
		else
			mytype = Protocol::PlayerType::PLAYER_TYPE_ORANGE_WOMAN;
		break;

	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::
		Printf(TEXT("ID:%s, type:%d%d"), *ID, SCHOOL, GENDER));


	// 회원가입 패킷에 넣어서 보내기
	{
		Protocol::C_MEMBERSHIP Pkt;
		Pkt.set_membership_id(myid);
		Pkt.set_membership_type(mytype);
		SEND_PACKET(Pkt);
	}



}

void USPLobbyWidget::Matching()
{
	// 매칭 패킷에 넣어서 보내기

}

void USPLobbyWidget::PublicChat(const FString& chat)
{
	// 채팅 패킷에 넣어서 보내기
}



void USPLobbyWidget::PrivateChat(const FString& school_type,const FString& chat)
{
	//if(school_type == 자신의 학교)
	// 채팅 패킷에 넣어서 보내기
	// else
	// 접속 불가 메세지 출력
}

void USPLobbyWidget::AddFriend(const FString& ID)
{
	// 친구 추가 패킷에 넣어서 보내기

}