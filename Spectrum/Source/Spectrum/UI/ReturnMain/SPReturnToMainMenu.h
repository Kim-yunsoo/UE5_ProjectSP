// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/SPColorScoreData.h"
#include "SPReturnToMainMenu.generated.h"

class UTextBlock;
class UImage;
class UMultiplayerSessionsSubsystem;
class UButton;

UCLASS()
class SPECTRUM_API USPReturnToMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	USPReturnToMainMenu(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnDestorySession(bool bWasSuccessful);

public:
	void MenuSetup(const TArray<FColorScoreData>& ColorScoreDataArray);
	void MenuTearDown();

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> ReturnButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> RankImage1;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> RankImage2;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> RankImage3;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Score1;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Score2;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Score3;
	
	UPROPERTY()
	TObjectPtr<UDataTable> DataAsset;

	void AddSchoolImage(const int32 Rank, const FString& AssetName, int32 InScore);
	

	UFUNCTION()
	void ReturnButtonClicked();

	UPROPERTY()
	TObjectPtr<UMultiplayerSessionsSubsystem> MultiplayerSessionsSubsystem;
	UPROPERTY()
	APlayerController* PlayerController;

};
