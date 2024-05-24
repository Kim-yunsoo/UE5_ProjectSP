// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPSelectWidget.generated.h"

/**
 * 
 */
class UEditableTextBox;
class UButton;

UCLASS()
class SPECTRUM_API USPSelectWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	USPSelectWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<UButton> GreenBut;

	UPROPERTY()
	TObjectPtr<UButton> OrangeBut;

	UPROPERTY()
	TObjectPtr<UButton> PurpleBut;

	UPROPERTY()
	TObjectPtr<UButton> ReadyBut;

	UPROPERTY()
	TObjectPtr<UButton> ManBut;

	UPROPERTY()
	TObjectPtr<UButton> WomanBut;

	UPROPERTY()
	TObjectPtr<UEditableTextBox> NickNameText;
	
	uint8 bIsSelectColor;
	uint8 bIsSelectGender;
	uint8 bIsReady;
	uint8 bIsSendNickName;
	

	UFUNCTION()
	void OnNickNameCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnClickGreenBut();

	UFUNCTION()
	void OnClickOrangeBut();

	UFUNCTION()
	void OnClickPurpleBut();

	UFUNCTION()
	void OnClickManBut();

	UFUNCTION()
	void OnClickWomanBut();

	UFUNCTION()
	void OnClickReadyBut();
};
