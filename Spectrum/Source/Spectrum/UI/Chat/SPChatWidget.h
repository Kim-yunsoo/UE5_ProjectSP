// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPChatWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API USPChatWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	UPROPERTY()
	TObjectPtr<class UScrollBox> MessagesList;
	
	UPROPERTY()
	TObjectPtr<class UEditableTextBox> MessageBox;

	UFUNCTION()
	void OnChatTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

public:
	void AddMessageOntoMessagesList(const FString& Sender, const FString& Message);

	void SetFousOnChat();

	UPROPERTY()
	uint8 bIsFocused;

	void ActiveChat(bool bIsActiveChat);

};
