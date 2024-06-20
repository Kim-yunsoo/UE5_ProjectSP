// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPGlobalEnum.h"
#include "Blueprint/UserWidget.h"
#include "SPHUDWidget.generated.h"


class UImage;
class UTextBlock;
class USPItemBase;
struct FInteractableData;
class USPInteractionWidget;
class USPMainMenu;


UCLASS()
class SPECTRUM_API USPHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	USPHUDWidget(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void NativeConstruct() override;


protected:
	UPROPERTY()
	TObjectPtr<class USPTargetUI> TargetUI;
	
	UPROPERTY()
	TObjectPtr<class USPInteractionWidget> InteractionWidget;

	UPROPERTY()
	TObjectPtr<class USPMakingPotionWidget> MakingPotionWidget;

	UPROPERTY()
	TObjectPtr<class USPManualWidget> ManualWidget;
	
	UPROPERTY()
	TObjectPtr<class USPSkillWidget> SlowSkillWidget;


	UPROPERTY()
	TObjectPtr<class USPSkillWidget> IceSkillWidget;

	UPROPERTY()
	TObjectPtr<class USPSkillWidget> TeleSkillWidget;

	UPROPERTY()
	TObjectPtr<class USPScoreWidget> ScoreWidget;

	UPROPERTY()
	TObjectPtr<class USPKeyWidget> KeyWidget;

	UPROPERTY()
	TObjectPtr<class USPGameTimeWidget> GameTimeWidget;
	
	UPROPERTY()
	TObjectPtr<class USPChatWidget> ChatWidget;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SpectrumText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> AIText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> AlarmImage;
	

	UPROPERTY()
	TObjectPtr<class USPLoadingWidget> LoadingWidget;
	
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<USPInteractionWidget> InteractionWidgetClass;



	/////////////////

	void ToggleMouse(bool bIsShowMouse);
	void UpdateTarget(bool bIsShowUI);
	void UpdateTime(float CountdownTime);
	void UpdateSlowCDTime(float NewCurrentTime);
	void UpdateMakingPotionWidget(bool bIsVisible);
	void UpdateManualWidget(bool bIsVisible);
	void UpdateIceCDTime(float NewCurrentTime);
	void UpdateTeleCDTime(float NewCurrentTime);
	void UpdateScore(const ColorType& Mycolor, const int32 Score);
	void ShowInteractionWidget();
	void HideInteractionWidget();
	void UpdateInteractionWidget(const FInteractableData* InteractableData);
	void ToggleKeyWidget(bool bIsVisible);
	void ClearMakingWieget();
	void MakingPotionWieget(USPItemBase* Item);

	void UpdateChatting(const FString& Sender, const FString& Message);

	void UpdatePotionUI(const int32 Index);


	void ShowChat();
	void UpdateShowUIText();
	void UpdateAIInfoText();
	void HideLoadingWidget();

protected:
};
