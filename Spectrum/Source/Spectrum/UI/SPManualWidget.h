// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPManualWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class SPECTRUM_API USPManualWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* SpectrumLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* Manual;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* Back;

	UPROPERTY(VisibleAnywhere, Category = "Manual", meta=(BindWidget))
	TObjectPtr<UImage> MainImage;
	
	UPROPERTY(VisibleAnywhere, Category = "Manual", meta=(BindWidget))
	TObjectPtr<UImage> ManualImage;
	
	UPROPERTY(VisibleAnywhere, Category = "Manual", meta=(BindWidget))
	TObjectPtr<UImage> SpectrumLocationImage;

	virtual void NativeConstruct() override;
public:

protected:
	UFUNCTION()
	void SpectrumLocationCallBack();
	UFUNCTION()
	void ManualCallBack();
	UFUNCTION()
	void BackCallBack();
};
