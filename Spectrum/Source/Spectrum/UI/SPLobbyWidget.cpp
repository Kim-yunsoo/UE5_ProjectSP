// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPLobbyWidget.h"
#include "Spectrum.h"
#include "ClientPacketHandler.h"
#include "Protocol.pb.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")


 PlayerInfomation MyPlayerInfo;
 LobbyInfomation MyLobbyInfo;
 std::string ServerIP;
 bool matching_H;
 bool matching_C;
 int pir_num_G = -1;
 int pir_num_P = -1;
 int pir_num_O = -1;
 extern FString IpAddress;

USPLobbyWidget::USPLobbyWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	School = -1;
	Gender = -1;
	//UserId = TEXT("");
	//ChatMessage = TEXT("");
	//Public_Chat_Id = "";
	//Public_Chat_Msg = "";
	//school_O = 2;
	//school_P = 1;
	//school_G = 0;

	//const FInputModeGameAndUI InputMode;
	//GetOwningPlayer()->SetInputMode(InputMode);
	//GetOwningPlayer()->SetShowMouseCursor(true);
	matching_H = false;
	matching_C = false;
}

std::string USPLobbyWidget::GetMyIP()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	char hostname[256];
	if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
		return "fail";
	}

	struct addrinfo hints, * res, * ptr;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; // IPv4 주소만 요청
	if (getaddrinfo(hostname, NULL, &hints, &res) != 0) {
		//printf("Error resolving hostname\n");
		WSACleanup();
		return "fail";
	}

		char ip[INET_ADDRSTRLEN];
	for (ptr = res; ptr != NULL; ptr = ptr->ai_next) {
		inet_ntop(AF_INET, &((struct sockaddr_in*)ptr->ai_addr)->sin_addr, ip, sizeof(ip));
		//printf("IP Address: %s\n", ip);
	}

	freeaddrinfo(res);
	WSACleanup();

	return ip;
}

//void USPLobbyWidget::CreateServer()
//{
//	UGameplayStatics::OpenLevel(GetWorld(), FName("Building"), true, ((FString)(L"Listen")));
//}
//
//void USPLobbyWidget::JoinServer()
//{
//	UGameplayStatics::OpenLevel(GetWorld(), FName("127.0.0.1")); // Level 이름 대신 IP 주소.
//}

void USPLobbyWidget::SetIpAddress(const FString& InIpAddress)
{
	IpAddress = InIpAddress;
	ready = true;
}

bool USPLobbyWidget::readyIOCP()
{
	return ready;
}

/*************************
*     로그인 함수들      *
**************************/
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
	bool success = MyPlayerInfo.issuccess;
	MyPlayerInfo.issuccess = false;

	if (success)
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

/*************************
*     게임 매칭 함수들   *
**************************/

void USPLobbyWidget::CreateRoom()
{
	// 자신의 ip 주소를 가져와서 서버로 보내기
	std::string myip = GetMyIP();
	Protocol::C_CREATE_ROOM Pkt;
	Pkt.set_ip_address(myip);
	Pkt.set_membership_id(MyPlayerInfo.my_membership_id);
	Pkt.set_membership_type(MyPlayerInfo.player_type);
	Pkt.set_school_type(MyPlayerInfo.school_type);
	SEND_PACKET(Pkt);
}
void USPLobbyWidget::JoinRoom()
{
	// 자신의 정보를 서버로 보내기
	Protocol::C_ENTER_ROOM Pkt;
	Pkt.set_membership_id(MyPlayerInfo.my_membership_id);
	Pkt.set_membership_type(MyPlayerInfo.player_type);
	Pkt.set_school_type(MyPlayerInfo.school_type);
	SEND_PACKET(Pkt);
}
FString USPLobbyWidget::MatchingServer()
{
	// 매칭할 서버
	//return TEXT("Open 192.168.45.163");
	FString ip = FString(UTF8_TO_TCHAR(ServerIP.c_str()));
	FString return_ip = TEXT("Open ") + ip;
	//if(matching_C == true)
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("%s"), *ip));
	//return TEXT("Open 127.0.0.1");
	//return TEXT("Open 192.168.1.21");
	return TEXT("Open "+ip);
	//return return_ip;
}
bool USPLobbyWidget::MatchingSuccess_H()
{
	//if(matching_H)
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("0000000000000000")));

	return matching_H;
}
bool USPLobbyWidget::MatchingSuccess_C()
{
	//if (matching_C)
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("0000000000000000")));

	return matching_C;
}
/*************************
*     전체 채팅 함수들   *
**************************/

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

/*************************
*     학교 채팅 함수들   *
**************************/


/// /////////////////////////////
FString USPLobbyWidget::PrivateChatUpdate_G_0()
{
	FString Private_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[0][0].msg_id.c_str()));
	FString Private_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[0][0].msg.c_str()));
	return Private_Chat_Id + TEXT(" : ") + Private_Chat_Msg;
}
FString USPLobbyWidget::PrivateChatUpdate_G_1()
{
	FString Private_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[0][1].msg_id.c_str()));
	FString Private_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[0][1].msg.c_str()));
	return Private_Chat_Id + TEXT(" : ") + Private_Chat_Msg;
}
FString USPLobbyWidget::PrivateChatUpdate_G_2()
{
	FString Private_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[0][2].msg_id.c_str()));
	FString Private_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[0][2].msg.c_str()));
	return Private_Chat_Id + TEXT(" : ") + Private_Chat_Msg;
}
FString USPLobbyWidget::PrivateChatUpdate_G_3()
{
	FString Private_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[0][3].msg_id.c_str()));
	FString Private_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[0][3].msg.c_str()));
	return Private_Chat_Id + TEXT(" : ") + Private_Chat_Msg;
}
FString USPLobbyWidget::PrivateChatUpdate_G_4()
{
	FString Private_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[0][4].msg_id.c_str()));
	FString Private_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[0][4].msg.c_str()));
	return Private_Chat_Id + TEXT(" : ") + Private_Chat_Msg;
}
FString USPLobbyWidget::PrivateChatUpdate_G_5()
{
	FString Private_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[0][5].msg_id.c_str()));
	FString Private_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[0][5].msg.c_str()));
	return Private_Chat_Id + TEXT(" : ") + Private_Chat_Msg;
}
FString USPLobbyWidget::PrivateChatUpdate_G_6()
{
	FString Private_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[0][6].msg_id.c_str()));
	FString Private_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[0][6].msg.c_str()));
	return Private_Chat_Id + TEXT(" : ") + Private_Chat_Msg;
}

FString USPLobbyWidget::PrivateChatUpdate_P_0()
{
	FString Private_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[1][0].msg_id.c_str()));
	FString Private_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[1][0].msg.c_str()));
	return Private_Chat_Id + TEXT(" : ") + Private_Chat_Msg;
}
FString USPLobbyWidget::PrivateChatUpdate_P_1()
{
	FString Private_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[1][1].msg_id.c_str()));
	FString Private_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[1][1].msg.c_str()));
	return Private_Chat_Id + TEXT(" : ") + Private_Chat_Msg;
}
FString USPLobbyWidget::PrivateChatUpdate_P_2()
{
	FString Private_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[1][2].msg_id.c_str()));
	FString Private_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[1][2].msg.c_str()));
	return Private_Chat_Id + TEXT(" : ") + Private_Chat_Msg;
}
FString USPLobbyWidget::PrivateChatUpdate_P_3()
{
	FString Private_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[1][3].msg_id.c_str()));
	FString Private_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[1][3].msg.c_str()));
	return Private_Chat_Id + TEXT(" : ") + Private_Chat_Msg;
}
FString USPLobbyWidget::PrivateChatUpdate_P_4()
{
	FString Private_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[1][4].msg_id.c_str()));
	FString Private_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[1][4].msg.c_str()));
	return Private_Chat_Id + TEXT(" : ") + Private_Chat_Msg;
}
FString USPLobbyWidget::PrivateChatUpdate_P_5()
{
	FString Private_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[1][5].msg_id.c_str()));
	FString Private_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[1][5].msg.c_str()));
	return Private_Chat_Id + TEXT(" : ") + Private_Chat_Msg;
}
FString USPLobbyWidget::PrivateChatUpdate_P_6()
{
	FString Private_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[1][6].msg_id.c_str()));
	FString Private_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[1][6].msg.c_str()));
	return Private_Chat_Id + TEXT(" : ") + Private_Chat_Msg;
}

FString USPLobbyWidget::PrivateChatUpdate_O_0()
{
	FString Private_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[2][0].msg_id.c_str()));
	FString Private_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[2][0].msg.c_str()));
	return Private_Chat_Id + TEXT(" : ") + Private_Chat_Msg;
}
FString USPLobbyWidget::PrivateChatUpdate_O_1()
{
	FString Private_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[2][1].msg_id.c_str()));
	FString Private_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[2][1].msg.c_str()));
	return Private_Chat_Id + TEXT(" : ") + Private_Chat_Msg;
}
FString USPLobbyWidget::PrivateChatUpdate_O_2()
{
	FString Private_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[2][2].msg_id.c_str()));
	FString Private_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[2][2].msg.c_str()));
	return Private_Chat_Id + TEXT(" : ") + Private_Chat_Msg;
}
FString USPLobbyWidget::PrivateChatUpdate_O_3()
{
	FString Private_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[2][3].msg_id.c_str()));
	FString Private_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[2][3].msg.c_str()));
	return Private_Chat_Id + TEXT(" : ") + Private_Chat_Msg;
}
FString USPLobbyWidget::PrivateChatUpdate_O_4()
{
	FString Private_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[2][4].msg_id.c_str()));
	FString Private_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[2][4].msg.c_str()));
	return Private_Chat_Id + TEXT(" : ") + Private_Chat_Msg;
}
FString USPLobbyWidget::PrivateChatUpdate_O_5()
{
	FString Private_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[2][5].msg_id.c_str()));
	FString Private_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[2][5].msg.c_str()));
	return Private_Chat_Id + TEXT(" : ") + Private_Chat_Msg;
}
FString USPLobbyWidget::PrivateChatUpdate_O_6()
{
	FString Private_Chat_Id = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[2][6].msg_id.c_str()));
	FString Private_Chat_Msg = FString(UTF8_TO_TCHAR(MyLobbyInfo.privateBoard[2][6].msg.c_str()));
	return Private_Chat_Id + TEXT(" : ") + Private_Chat_Msg;
}


void USPLobbyWidget::PrivateChat(const int& school_num, const FString& chat)
{
	if (school_num == MyPlayerInfo.school_num_type)
	{
		std::string mychat = TCHAR_TO_UTF8(*chat);

		// 채팅 패킷에 넣어서 보내기
		Protocol::C_PRIVATE_CHAT Pkt;
		Pkt.set_membership_id(MyPlayerInfo.my_membership_id);
		Pkt.set_msg(mychat);
		Pkt.set_membership_type(MyPlayerInfo.school_type);

		//switch (school_num){
		//case 0: //그린
		//	Pkt.set_membership_type(Protocol::SCHOOL_TYPE_GREEN);
		//	break;
		//case 1: //퍼플
		//	Pkt.set_membership_type(Protocol::SCHOOL_TYPE_PURPLE);
		//	break;
		//case 2: //오렌지
		//	Pkt.set_membership_type(Protocol::SCHOOL_TYPE_ORANGE);
		//	break;}

		SEND_PACKET(Pkt);
	}

	//if(school_type == 자신의 학교)
	// 채팅 패킷에 넣어서 보내기
	// else
	// 접속 불가 메세지 출력
}

void USPLobbyWidget::AddFriend(const FString& ID)
{
	// 친구 추가 패킷에 넣어서 보내기
}

//bool USPLobbyWidget::SchoolSuccess(const int& num)
//{
//	return MyPlayerInfo.pri_membership_type == num;
//}