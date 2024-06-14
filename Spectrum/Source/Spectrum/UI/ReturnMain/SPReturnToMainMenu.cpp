// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ReturnMain/SPReturnToMainMenu.h"
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "SPGameModeBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/SchoolImageStruct.h"
#include "GameFramework/GameModeBase.h"

USPReturnToMainMenu::USPReturnToMainMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> TableRef(
		TEXT("/Script/Engine.DataTable'/Game/Spectrum/ColorData/DT_SchoolTexture.DT_SchoolTexture'"));
	if (TableRef.Object)
	{
		DataAsset = TableRef.Object;
	}
}

void USPReturnToMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	///Script/Engine.DataTable'/Game/Spectrum/ColorData/DT_SchoolTexture.DT_SchoolTexture'
}


void USPReturnToMainMenu::MenuSetup(const TArray<FColorScoreData>& ColorScoreDataArray)
{
	TArray<FColorScoreData> MutableArray = ColorScoreDataArray; //���� �����Ѵ�.

	MutableArray.Sort([](const FColorScoreData& A, const FColorScoreData& B)
	{
		return A.Score > B.Score; // �������� ����
	});


	for (int32 i = 0; i < MutableArray.Num(); ++i)
	{
		FString AssetColor = UEnum::GetValueAsString(MutableArray[i].Color);
		AddSchoolImage(i, AssetColor,MutableArray[i].Score);
	}
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);
	//bIsFocusable = true; // ������ ���� �� �� �ִ�.
	UWorld* World = GetWorld();
	if (World)
	{
		PlayerController = ((PlayerController == nullptr) ? World->GetFirstPlayerController() : PlayerController);
		if (PlayerController)
		{
			FInputModeGameAndUI InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	if (ReturnButton && !ReturnButton->OnClicked.IsBound())
	{
		ReturnButton->OnClicked.AddDynamic(this, &USPReturnToMainMenu::ReturnButtonClicked);
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if (MultiplayerSessionsSubsystem)
		{
			MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(
				this, &USPReturnToMainMenu::OnDestorySession);
		}
	}
}

void USPReturnToMainMenu::AddSchoolImage(const int32 Rank, const FString& AssetName, int32 InScore)
{
	//	FString CountdownText = FString::Printf(TEXT("%02d:%02d"),Minutes,Seconds);

	FSchoolColorData* Data = DataAsset->FindRow<FSchoolColorData>(FName(*AssetName), "");
	FString StringScore = FString::Printf(TEXT("%d"), InScore);
	if (Data)
	{
		FSlateBrush MyBrush;
		MyBrush.SetResourceObject(Data->Texture); //�̹��� �ٲٱ� 
		if (Rank == 0)
		{
			RankImage1->SetBrush(MyBrush);
			Score1->SetText(FText::FromString(StringScore));
		}
		else if (Rank == 1)
		{
			RankImage2->SetBrush(MyBrush);
			Score2->SetText(FText::FromString(StringScore));
		}
		else
		{
			RankImage3->SetBrush(MyBrush);
			Score3->SetText(FText::FromString(StringScore));
		}
	}
}

void USPReturnToMainMenu::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World)
	{
		PlayerController = (PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController);

		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
	if (ReturnButton && ReturnButton->OnClicked.IsBound()) //������ ���ε��� ���� �� ���ε��ϴ� �ߺ������� ���ϱ� ���ؼ� �����.
	{
		ReturnButton->OnClicked.RemoveDynamic(this, &USPReturnToMainMenu::ReturnButtonClicked);
	}
	if (MultiplayerSessionsSubsystem && MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.IsBound())
	{
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.RemoveDynamic(
			this, &USPReturnToMainMenu::OnDestorySession);
	}
}


void USPReturnToMainMenu::ReturnButtonClicked() //������ �����ϴ� ��ư�� �������Ѵ�. 
{
	ReturnButton->SetIsEnabled(false);
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->DestroySession();
	}
}


void USPReturnToMainMenu::OnDestorySession(bool bWasSuccessful) //������ Destroy�Ǹ� ��������Ʈ�� ���� ȣ��ȴ�. 
{
	if (!bWasSuccessful)
	{
		ReturnButton->SetIsEnabled(true);
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>();
		if (GameMode) //�������� ȣ��� ���
		{
			GameMode->ReturnToMainMenuHost();
		}
		else //Ŭ���̾�Ʈ���� ȣ��� ���
		{
			PlayerController = (PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController);

			if (PlayerController)
			{
				PlayerController->ClientReturnToMainMenuWithTextReason(FText());
			}
		}
	}
}
