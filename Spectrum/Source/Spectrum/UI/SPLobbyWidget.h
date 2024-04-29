// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPLobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API USPLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 시작 위젯
	// 아이디 받는 함수
	UFUNCTION(BlueprintCallable, Category = "ID Verification")
	bool ValidateID(const FString& ID);
	
	// 회원가입 위젯
	UFUNCTION(BlueprintCallable, Category = "membership")
	void Membership(const FString& ID, const FString& SCHOOL, const FString& GENDER);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "membership")
	int School;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "membership")
	int Gender;

	// 매칭 위젯
	UFUNCTION(BlueprintCallable, Category = "Matching")
	void Matching();

	// 전체 게시판 위젯
	UFUNCTION(BlueprintCallable, Category = "PublicChat")
	void PublicChat(const FString& chat);

	// 학교 게시판 위젯
	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	void PrivateChat(const FString& school_type, const FString& chat);

	// 친구 추가 위젯
	UFUNCTION(BlueprintCallable, Category = "Friend")
	void AddFriend(const FString& ID);


};
