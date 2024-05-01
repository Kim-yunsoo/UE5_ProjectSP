// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpectrumGameInstance.h"
#include "SPLobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API USPLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

	USPLobbyWidget(const FObjectInitializer& ObjectInitializer);

public:
	// ���� �α��� ����
	UFUNCTION(BlueprintCallable, Category = "ID Verification")	// ���̵� �޴� �Լ�
	void ValidateID(const FString& ID);

	UFUNCTION(BlueprintCallable, Category = "ID Verification")	// ���̵� �޴� �Լ�
	bool LoginSuccess();
	
	// ȸ������ ����
	UFUNCTION(BlueprintCallable, Category = "membership")
	void Membership(const FString& ID, const int& SCHOOL, const int& GENDER);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "membership")
	int School;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "membership")
	int Gender;

	// ��Ī ����
	UFUNCTION(BlueprintCallable, Category = "Matching")
	void Matching();

	// ��ü �Խ��� ����
	UFUNCTION(BlueprintCallable, Category = "PublicChat")
	void PublicChat(const FString& chat);

	// �б� �Խ��� ����
	UFUNCTION(BlueprintCallable, Category = "PrivateChat")
	void PrivateChat(const FString& school_type, const FString& chat);

	// ģ�� �߰� ����
	UFUNCTION(BlueprintCallable, Category = "Friend")
	void AddFriend(const FString& ID);


};
