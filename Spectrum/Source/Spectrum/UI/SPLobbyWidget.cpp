// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPLobbyWidget.h"
#include "Protocol.pb.h"
#include "Spectrum.h"


bool USPLobbyWidget::ValidateID(const FString& ID)
{
    // 아이디 화면에 출력(확인용)
    //if( !ID.IsEmpty())
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ID:%s"), *ID));

	std::string myid = TCHAR_TO_UTF8(*ID);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ID:%s"), myid));

	return myid == "a";

	// 아이디가 DB에 있는지 확인하기 위해 서버로 보냄
	{ // 로그인	패킷 보내기
		Protocol::C_LOGIN Pkt;
		Pkt.set_membership_id("test");

		SEND_PACKET(Pkt);
	}

	// 없으면 회원가입 위젯으로 넘어감 false 반환
	//return false;
	
	//있으면 true 반환
	//return true;
}


void USPLobbyWidget::Membership(const FString& ID, const FString& SCHOOL, const FString& GENDER)
{
	// 회원가입 패킷에 넣어서 보내기




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