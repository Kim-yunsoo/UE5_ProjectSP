// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPManualWidget.generated.h"

class USizeBox;
class UImage;
/**
 * 
 */
UCLASS()
class SPECTRUM_API USPManualWidget : public UUserWidget
{
	GENERATED_BODY()
	// USPManualWidget(const FObjectInitializer& ObjectInitializer);
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

	UPROPERTY(EditAnywhere)
	USoundBase* ClickOn;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> SizeBox0;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> SizeBox1;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> SizeBox2;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> SizeBox3;

	UPROPERTY()
	TArray<USizeBox*> SizeBoxArray;
public:
	UPROPERTY()
	TObjectPtr<UTexture> SpectrumTexture;
	
	virtual void NativeConstruct() override;
public:
	void UpdatePotionUI(const int32 Index);

protected:
	UFUNCTION()
	void SpectrumLocationCallBack();
	UFUNCTION()
	void ManualCallBack();
	UFUNCTION()
	void BackCallBack();

};
