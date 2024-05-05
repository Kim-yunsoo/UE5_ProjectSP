// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPLobbyWidget.h"
#include "Spectrum.h"
#include "ClientPacketHandler.h"
#include "Protocol.pb.h"

//extern bool issuccess;
//extern std::string my_membership_id;
//extern std::string mag_id;
//extern std::string mag;
//extern std::string msg_num;
//extern int school_type;
//extern std::string pri_mag_id;
//extern std::string pri_mag;
//extern int pri_membership_type;

 PlayerInfomation MyPlayerInfo;
 LobbyInfomation MyLobbyInfo;


USPLobbyWidget::USPLobbyWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	School = -1;
	Gender = -1;
	//UserId = TEXT("");
	//ChatMessage = TEXT("");
	//Public_Chat_Id = "";
	//Public_Chat_Msg = "";
	school_O = 2;
	school_P = 1;
	school_G = 0;

	//const FInputModeGameAndUI InputMode;
	//GetOwningPlayer()->SetInputMode(InputMode);
	//GetOwningPlayer()->SetShowMouseCursor(true);



}

void USPLobbyWidget::CreateServer()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("Building"), true, ((FString)(L"Listen")));
}

void USPLobbyWidget::JoinServer()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("127.0.0.1")); // Level 이름 대신 IP 주소.
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
	if (MyPlayerInfo.issuccess)
		return true;
	else
		return false;
}


void USPLobbyWidget::Membership(const FString& ID, const int& SCHOOL, const int& GENDER)
{
	std::string myid = TCHAR_TO_UTF8(*ID);
	Protocol::PlayerType mytype = Protocol::PlayerType::PLAYER_TYPE_NONE;

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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
		FString::Printf(TEXT("ID:%s, type:%d%d"), *ID, SCHOOL, GENDER));


	// 회원가입 패킷에 넣어서 보내기
	if(mytype != Protocol::PlayerType::PLAYER_TYPE_NONE)
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

void USPLobbyWidget::PublicChat(const FString& num, const FString& chat)
{

	std::string mychat = TCHAR_TO_UTF8(*chat);

	// 채팅 패킷에 넣어서 보내기
	Protocol::C_PUBLIC_CHAT Pkt;
	Pkt.set_membership_id(MyPlayerInfo.my_membership_id);
	Pkt.set_msg(mychat);
	Pkt.set_msg_num(TCHAR_TO_UTF8(*num));

	SEND_PACKET(Pkt);

}



FString USPLobbyWidget::PublicChatUpdate_0()
{
	FString Public_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.publicBoard[0].msg_id.c_str()));
	FString Public_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.publicBoard[0].msg.c_str()));
	return Public_Chat_Id + TEXT(" : ") + Public_Chat_Msg;
}

FString USPLobbyWidget::PublicChatUpdate_1()
{
	FString Public_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.publicBoard[1].msg_id.c_str()));
	FString Public_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.publicBoard[1].msg.c_str()));
	return Public_Chat_Id + TEXT(" : ") + Public_Chat_Msg;
}

FString USPLobbyWidget::PublicChatUpdate_2()
{
	FString Public_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.publicBoard[2].msg_id.c_str()));
	FString Public_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.publicBoard[2].msg.c_str()));
	return Public_Chat_Id + TEXT(" : ") + Public_Chat_Msg;
}

FString USPLobbyWidget::PublicChatUpdate_3()
{
	FString Public_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.publicBoard[3].msg_id.c_str()));
	FString Public_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.publicBoard[3].msg.c_str()));
	return Public_Chat_Id + TEXT(" : ") + Public_Chat_Msg;
}

FString USPLobbyWidget::PublicChatUpdate_4()
{
	FString Public_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.publicBoard[4].msg_id.c_str()));
	FString Public_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.publicBoard[4].msg.c_str()));
	return Public_Chat_Id + TEXT(" : ") + Public_Chat_Msg;
}

FString USPLobbyWidget::PublicChatUpdate_5()
{
	FString Public_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.publicBoard[5].msg_id.c_str()));
	FString Public_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.publicBoard[5].msg.c_str()));
	return Public_Chat_Id + TEXT(" : ") + Public_Chat_Msg;
}

FString USPLobbyWidget::PublicChatUpdate_6()
{
	FString Public_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.publicBoard[6].msg_id.c_str()));
	FString Public_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.publicBoard[6].msg.c_str()));
	return Public_Chat_Id + TEXT(" : ") + Public_Chat_Msg;
}

FString USPLobbyWidget::PublicChatUpdate_7()
{
	FString Public_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.publicBoard[7].msg_id.c_str()));
	FString Public_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.publicBoard[7].msg.c_str()));
	return Public_Chat_Id + TEXT(" : ") + Public_Chat_Msg;
}

FString USPLobbyWidget::PrivateChatUpdate_G()
{
	FString Private_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[0].msg_id.c_str()));
	FString Private_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[0].msg.c_str()));
	return Private_Chat_Id + TEXT(" : ") + Private_Chat_Msg;
}

FString USPLobbyWidget::PrivateChatUpdate_P()
{
	FString Private_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[1].msg_id.c_str()));
	FString Private_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[1].msg.c_str()));
	return Private_Chat_Id + TEXT(" : ") + Private_Chat_Msg;
}

FString USPLobbyWidget::PrivateChatUpdate_O()
{
	FString Private_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[2].msg_id.c_str()));
	FString Private_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[2].msg.c_str()));
	return Private_Chat_Id + TEXT(" : ") + Private_Chat_Msg;
}



void USPLobbyWidget::PrivateChat(const int& school_num, const FString& chat)
{
	if (school_num == MyPlayerInfo.school_type)
	{
		std::string mychat = TCHAR_TO_UTF8(*chat);

		// 채팅 패킷에 넣어서 보내기
		Protocol::C_PRIVATE_CHAT Pkt;
		Pkt.set_membership_id(MyPlayerInfo.my_membership_id);
		Pkt.set_msg(mychat);

		switch (school_num)
		{
		case 0: //그린
			Pkt.set_membership_type(Protocol::SCHOOL_TYPE_GREEN);
			break;

		case 1: //퍼플
			Pkt.set_membership_type(Protocol::SCHOOL_TYPE_PURPLE);

			break;
		case 2: //오렌지
			Pkt.set_membership_type(Protocol::SCHOOL_TYPE_ORANGE);

			break;

		}

		SEND_PACKET(Pkt);
	}

	//if(school_type == 자신의 학교)
	// 채팅 패킷에 넣어서 보내기
	// else
	// 접속 불가 메세지 출력
}

//FString USPLobbyWidget::PrivateChatUpdate()
//{
//	//if (pri_membership_type == school_num) {
//
//	//}
//	FString Private_Chat_Id = FString(UTF8_TO_TCHAR(MyPlayerInfo.pri_mag_id.c_str()));
//	FString Private_Chat_Msg = FString(UTF8_TO_TCHAR(MyPlayerInfo.pri_mag.c_str()));
//
//	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s, %s"), *Public_Chat_Id, *Public_Chat_Msg));
//	return Private_Chat_Id + TEXT(" : ") + Private_Chat_Msg;
//
//}






void USPLobbyWidget::AddFriend(const FString& ID)
{
	// 친구 추가 패킷에 넣어서 보내기
}

//bool USPLobbyWidget::SchoolSuccess(const int& num)
//{
//	return MyPlayerInfo.pri_membership_type == num;
//}