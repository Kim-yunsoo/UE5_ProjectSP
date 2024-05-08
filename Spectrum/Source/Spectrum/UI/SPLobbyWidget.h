// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpectrumGameInstance.h"
#include <array>
#include <string>
#include "Kismet/GameplayStatics.h"
#include "SPLobbyWidget.generated.h"


class PlayerInfomation
{
public:
	bool issuccess;
	std::string my_membership_id;	
	Protocol::SchoolType school_type;
	int school_num_type;
	Protocol::PlayerType player_type;
};

class publicchatInfo
{
public:
	std::string msg_id;
	std::string msg;
	std::string msg_num;

};

class privatechatInfo
{
public:
	std::string msg_id;
	std::string msg;
	Protocol::SchoolType membership_type;
};


class LobbyInfomation
{
public:
	// 퍼블릭 게시판 정보
	std::array <publicchatInfo, 8> publicBoard;

	// 프라이빗 게시판 정보
	std::array<std::array<privatechatInfo, 7>, 3> privateBoard;

	//std::array <privatechatInfo, 7> privateBoard_G;

};


/**
 * 
 */
UCLASS()
class SPECTRUM_API USPLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

	USPLobbyWidget(const FObjectInitializer& ObjectInitializer);

public:
	std::string GetMyIP();

//public:
//	UFUNCTION(BlueprintCallable, Category = "Server")
//	void CreateServer();
//
//	UFUNCTION(BlueprintCallable, Category = "Server")
//	void JoinServer();

public:
	UFUNCTION(BlueprintCallable, Category = "SetIOCP")
	void SetIpAddress(const FString& InIpAddress);

	UFUNCTION(BlueprintCallable, Category = "SetIOCP")
	bool readyIOCP();

	bool ready = false;


	// 시작 로그인 위젯
	UFUNCTION(BlueprintCallable, Category = "ID Verification")	// 아이디 받는 함수
	void ValidateID(const FString& ID);

	UFUNCTION(BlueprintCallable, Category = "ID Verification")	// 아이디 받는 함수
	bool LoginSuccess();
	
	// 회원가입 위젯
	UFUNCTION(BlueprintCallable, Category = "membership")
	void Membership(const FString& ID, const int& SCHOOL, const int& GENDER);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "membership")
	int School;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "membership")
	int Gender;

	// 매칭 위젯
	UFUNCTION(BlueprintCallable, Category = "Matching")
	void CreateRoom();

	UFUNCTION(BlueprintCallable, Category = "Matching")
	void JoinRoom();

	UFUNCTION(BlueprintCallable, Category = "Matching")
	FString MatchingServer();

	UFUNCTION(BlueprintCallable, Category = "Matching")
	bool MatchingSuccess_H();

	UFUNCTION(BlueprintCallable, Category = "Matching")
	bool MatchingSuccess_C();

	// 전체 게시판 위젯
	UFUNCTION(BlueprintCallable, Category = "PublicChat")
	void PublicChat(const FString& num, const FString& chat);

	UFUNCTION(BlueprintCallable, Category = "PublicChat")
	FString PublicChatUpdate_0();

	UFUNCTION(BlueprintCallable, Category = "PublicChat")
	FString PublicChatUpdate_1();

	UFUNCTION(BlueprintCallable, Category = "PublicChat")
	FString PublicChatUpdate_2();

	UFUNCTION(BlueprintCallable, Category = "PublicChat")
	FString PublicChatUpdate_3();

	UFUNCTION(BlueprintCallable, Category = "PublicChat")
	FString PublicChatUpdate_4();

	UFUNCTION(BlueprintCallable, Category = "PublicChat")
	FString PublicChatUpdate_5();

	UFUNCTION(BlueprintCallable, Category = "PublicChat")
	FString PublicChatUpdate_6();

	UFUNCTION(BlueprintCallable, Category = "PublicChat")
	FString PublicChatUpdate_7();


	//UFUNCTION(BlueprintCallable, Category = "PublicChat")	// 해당 게시판이 맞는지
	//bool ChatSuccess(const FString& num);

	UPROPERTY(BlueprintReadWrite, Category = "PublicChat")
	FString Pre_chat_0;

	UPROPERTY(BlueprintReadWrite, Category = "PublicChat")
	FString Pre_chat_1;
	UPROPERTY(BlueprintReadWrite, Category = "PublicChat")
	FString Pre_chat_2;
	UPROPERTY(BlueprintReadWrite, Category = "PublicChat")
	FString Pre_chat_3;
	UPROPERTY(BlueprintReadWrite, Category = "PublicChat")
	FString Pre_chat_4;
	UPROPERTY(BlueprintReadWrite, Category = "PublicChat")
	FString Pre_chat_5;
	UPROPERTY(BlueprintReadWrite, Category = "PublicChat")
	FString Pre_chat_6;
	UPROPERTY(BlueprintReadWrite, Category = "PublicChat")
	FString Pre_chat_7;


	// 학교 게시판 위젯
	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	void PrivateChat(const int& school_num, const FString& chat);

	UPROPERTY(BlueprintReadWrite, Category = "PrivateChat")
	int school_G=0;
	UPROPERTY(BlueprintReadWrite, Category = "PrivateChat")
	int school_P=1;
	UPROPERTY(BlueprintReadWrite, Category = "PrivateChat")
	int school_O=2;

	UPROPERTY(BlueprintReadWrite, Category = "PrivateChat")
	FString Pre_chat_O;
	UPROPERTY(BlueprintReadWrite, Category = "PrivateChat")
	FString Pre_chat_P;
	UPROPERTY(BlueprintReadWrite, Category = "PrivateChat")
	FString Pre_chat_G;

	//UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	//FString PrivateChatUpdate();

	//UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	//FString PrivateChatUpdate_G();

	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	FString PrivateChatUpdate_G_0();
	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	FString PrivateChatUpdate_G_1();
	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	FString PrivateChatUpdate_G_2();
	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	FString PrivateChatUpdate_G_3();
	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	FString PrivateChatUpdate_G_4();
	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	FString PrivateChatUpdate_G_5();
	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	FString PrivateChatUpdate_G_6();
	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	FString PrivateChatUpdate_P_0();
	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	FString PrivateChatUpdate_P_1();
	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	FString PrivateChatUpdate_P_2();
	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	FString PrivateChatUpdate_P_3();
	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	FString PrivateChatUpdate_P_4();
	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	FString PrivateChatUpdate_P_5();
	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	FString PrivateChatUpdate_P_6();
	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	FString PrivateChatUpdate_O_0();
	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	FString PrivateChatUpdate_O_1();
	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	FString PrivateChatUpdate_O_2();
	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	FString PrivateChatUpdate_O_3();
	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	FString PrivateChatUpdate_O_4();
	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	FString PrivateChatUpdate_O_5();
	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	FString PrivateChatUpdate_O_6();


	//UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	//FString PrivateChatUpdate_P();

	//UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	//FString PrivateChatUpdate_O();

	//UFUNCTION(BlueprintCallable, Category = "PrivateChat")	// 해당 게시판이 맞는지
	//bool SchoolSuccess(const int& num);


	// 친구 추가 위젯
	UFUNCTION(BlueprintCallable, Category = "Friend")
	void AddFriend(const FString& ID);


};

extern PlayerInfomation MyPlayerInfo;	// 자신의 정보 저장하는 구조체
extern LobbyInfomation MyLobbyInfo;	// 로비 정보 저장하는 구조체
extern bool matching_H;
extern bool matching_C;
extern int pir_num_G;
extern int pir_num_P;
extern int pir_num_O;