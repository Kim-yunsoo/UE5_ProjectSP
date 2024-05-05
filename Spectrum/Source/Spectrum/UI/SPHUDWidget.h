// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPGlobalEnum.h"
#include "Blueprint/UserWidget.h"
#include "SPHUDWidget.generated.h"


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
	TObjectPtr<class USPMainMenu> MainMenuWidget;
	
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
	TObjectPtr<class USPGameTimeWidget> GameTimeWidget;
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<USPMainMenu> MainMenuClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<USPInteractionWidget> InteractionWidgetClass;

	bool bIsMenuVisible;


	/////////////////
	void DisplayMenu();
	void HideMenu();
	void ToggleMenu();

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

	void ClearMakingWieget();
	void MakingPotionWieget(USPItemBase* Item);
protected:
};
