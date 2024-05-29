// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPManualWidget.h"
#include "SPHUDWidget.h"
#include "Character/SPCharacterPlayer.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"


USPManualWidget::USPManualWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture> TextureRef(TEXT("/Game/Spectrum/Yunsoo/Assets/T_Potion2"));
	if(TextureRef.Object)
	{
		SpectrumTexture = TextureRef.Object;
	}
}

void USPManualWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SpectrumLocation = Cast<UButton>(GetWidgetFromName(TEXT("SpectrumLocation")));
	Manual = Cast<UButton>(GetWidgetFromName(TEXT("Manual")));
	Back = Cast<UButton>(GetWidgetFromName(TEXT("Back")));
		
	SpectrumLocation->OnClicked.AddDynamic(this, &USPManualWidget::SpectrumLocationCallBack);
	Manual->OnClicked.AddDynamic(this, &USPManualWidget::ManualCallBack);
	Back->OnClicked.AddDynamic(this, &USPManualWidget::BackCallBack);
	MainImage->SetVisibility(ESlateVisibility::Visible);
	ClickOn = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Spectrum/Sound/ClickOn.ClickOn'"));

	SizeBoxArray={SizeBox0,SizeBox1,SizeBox2,SizeBox3};

}

void USPManualWidget::SpectrumLocationCallBack()
{
	SpectrumLocationImage->SetVisibility(ESlateVisibility::Visible);
	MainImage->SetVisibility(ESlateVisibility::Hidden);
	ManualImage->SetVisibility(ESlateVisibility::Hidden);
	
	UGameplayStatics::PlaySound2D(GetWorld(), ClickOn);
}

void USPManualWidget::ManualCallBack()
{
	SpectrumLocationImage->SetVisibility(ESlateVisibility::Hidden);
	MainImage->SetVisibility(ESlateVisibility::Hidden);
	ManualImage->SetVisibility(ESlateVisibility::Visible);
	UGameplayStatics::PlaySound2D(GetWorld(), ClickOn);

}

void USPManualWidget::BackCallBack()
{
	if(MainImage->IsVisible())
	{
		ASPCharacterPlayer* Player = Cast<ASPCharacterPlayer>(GetOwningPlayerPawn());
		Player->HUDWidget->UpdateManualWidget(false);
		
	}
	else
	{
		MainImage->SetVisibility(ESlateVisibility::Visible);
	}
	UGameplayStatics::PlaySound2D(GetWorld(), ClickOn);
}

void USPManualWidget::UpdatePotionUI(const int32 Index)
{
	UImage* PotionUI = NewObject<UImage>(this);
	FSlateBrush MyBrush;
	MyBrush = PotionUI->GetBrush();
	//MyBrush.SetResourceObject(SpectrumTexture); //이미지 바꾸기 
	PotionUI->SetBrush(MyBrush);
	
	SizeBoxArray[Index]->AddChild(PotionUI);
}
