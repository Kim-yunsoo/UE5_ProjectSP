// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ReturnMain/SPReturnToMainMenu.h"
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "GameFramework/GameModeBase.h"

void USPReturnToMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

}



void USPReturnToMainMenu::MenuSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);
	//bIsFocusable = true; // ������ ���� �� �� �ִ�.
	UWorld* World = GetWorld();
	if(World)
	{
		PlayerController = ((PlayerController==nullptr)? World->GetFirstPlayerController() : PlayerController); 
		if(PlayerController)
		{
			FInputModeGameAndUI InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	if(ReturnButton && !ReturnButton->OnClicked.IsBound())
	{
		ReturnButton->OnClicked.AddDynamic(this,&USPReturnToMainMenu::ReturnButtonClicked);
	}
	
	UGameInstance* GameInstance = GetGameInstance();
	if(GameInstance)
	{
		MultiplayerSessionsSubsystem =GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if(MultiplayerSessionsSubsystem)
		{
			MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this,&USPReturnToMainMenu::OnDestorySession);
		}
	}
}

void USPReturnToMainMenu::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if(World)
	{
		PlayerController = (PlayerController==nullptr? World->GetFirstPlayerController() : PlayerController); 

		if(PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
	if(ReturnButton && ReturnButton->OnClicked.IsBound()) //������ ���ε��� ���� �� ���ε��ϴ� �ߺ������� ���ϱ� ���ؼ� �����.
	{
		ReturnButton->OnClicked.RemoveDynamic(this,&USPReturnToMainMenu::ReturnButtonClicked);
	}
	if(MultiplayerSessionsSubsystem && MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.IsBound())
	{
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.RemoveDynamic(this,&USPReturnToMainMenu::OnDestorySession);
	}
}

void USPReturnToMainMenu::ReturnButtonClicked() //������ �����ϴ� ��ư�� �������Ѵ�. 
{
	ReturnButton->SetIsEnabled(false);
	if(MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->DestroySession();
	}
}


void USPReturnToMainMenu::OnDestorySession(bool bWasSuccessful) //������ Destroy�Ǹ� ��������Ʈ�� ���� ȣ��ȴ�. 
{
	if(!bWasSuccessful)
	{
		ReturnButton->SetIsEnabled(true);
		return;
	}
	
	UWorld* World = GetWorld();
	if(World)
	{
		AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>();
		if(GameMode) //�������� ȣ��� ���
		{
			GameMode->ReturnToMainMenuHost();
		}
		else //Ŭ���̾�Ʈ���� ȣ��� ���
		{
			PlayerController = (PlayerController==nullptr? World->GetFirstPlayerController() : PlayerController); 

			if(PlayerController)
			{
				PlayerController->ClientReturnToMainMenuWithTextReason(FText());
			}
		}
	}
}