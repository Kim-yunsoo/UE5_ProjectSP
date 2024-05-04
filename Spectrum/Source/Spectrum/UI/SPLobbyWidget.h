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
	int school_type;
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
	std::array <privatechatInfo, 3> privateBoard;

	// 매칭룸 정보

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
	UFUNCTION(BlueprintCallable, Category = "Server")
	void CreateServer();

	UFUNCTION(BlueprintCallable, Category = "Server")
	void JoinServer();

public:
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
	void Matching();

	UFUNCTION(BlueprintCallable, Category = "Matching")
	FString MatchingServer();

	// 전체 게시판 위젯
	UFUNCTION(BlueprintCallable, Category = "PublicChat")
	void PublicChat(const FString& num, const FString& chat);

	//UPROPERTY( BlueprintReadOnly, Category = "Chat Data")
	//FString UserId;

	//UPROPERTY( BlueprintReadOnly, Category = "Chat Data")
	//FString ChatMessage;

	//UFUNCTION(BlueprintCallable, Category = "PublicChat")
	//FString PublicChatUpdate(/*const FString& id, const FString& chat*/);

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
	int school_O;
	UPROPERTY(BlueprintReadWrite, Category = "PrivateChat")
	int school_P;
	UPROPERTY(BlueprintReadWrite, Category = "PrivateChat")
	int school_G;

	UPROPERTY(BlueprintReadWrite, Category = "PrivateChat")
	FString Pre_chat_O;
	UPROPERTY(BlueprintReadWrite, Category = "PrivateChat")
	FString Pre_chat_P;
	UPROPERTY(BlueprintReadWrite, Category = "PrivateChat")
	FString Pre_chat_G;

	//UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	//FString PrivateChatUpdate();

	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	FString PrivateChatUpdate_G();

	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	FString PrivateChatUpdate_P();

	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	FString PrivateChatUpdate_O();

	//UFUNCTION(BlueprintCallable, Category = "PrivateChat")	// 해당 게시판이 맞는지
	//bool SchoolSuccess(const int& num);


	// 친구 추가 위젯
	UFUNCTION(BlueprintCallable, Category = "Friend")
	void AddFriend(const FString& ID);


};

