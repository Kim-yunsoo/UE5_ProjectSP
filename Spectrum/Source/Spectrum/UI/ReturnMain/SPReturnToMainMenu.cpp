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
	//bIsFocusable = true; // 위젯에 집중 할 수 있다.
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
	if(ReturnButton && ReturnButton->OnClicked.IsBound()) //위에서 바인드한 것을 또 바인드하는 중복오류를 피하기 위해서 지운다.
	{
		ReturnButton->OnClicked.RemoveDynamic(this,&USPReturnToMainMenu::ReturnButtonClicked);
	}
	if(MultiplayerSessionsSubsystem && MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.IsBound())
	{
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.RemoveDynamic(this,&USPReturnToMainMenu::OnDestorySession);
	}
}

void USPReturnToMainMenu::ReturnButtonClicked() //세션을 종료하는 버튼을 만들어야한다. 
{
	ReturnButton->SetIsEnabled(false);
	if(MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->DestroySession();
	}
}


void USPReturnToMainMenu::OnDestorySession(bool bWasSuccessful) //세션이 Destroy되면 델리게이트를 통해 호출된다. 
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
		if(GameMode) //서버에서 호출된 경우
		{
			GameMode->ReturnToMainMenuHost();
		}
		else //클라이언트에서 호출된 경우
		{
			PlayerController = (PlayerController==nullptr? World->GetFirstPlayerController() : PlayerController); 

			if(PlayerController)
			{
				PlayerController->ClientReturnToMainMenuWithTextReason(FText());
			}
		}
	}
}