// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Chat/SPChatWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/SPCharacterPlayer.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "UI/SPHUDWidget.h"

void USPChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MessagesList = Cast<UScrollBox>(GetWidgetFromName(TEXT("MessagesList")));
	MessageBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("MessageBox")));

	//델리게이트 연결 
	MessageBox->OnTextCommitted.AddDynamic(this, &USPChatWidget::OnChatTextCommitted);
	MessageBox->SetClearKeyboardFocusOnCommit(false);

}

void USPChatWidget::OnChatTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	
	if (CommitMethod == ETextCommit::OnEnter)
	{
		ASPCharacterPlayer* Player = Cast<ASPCharacterPlayer>(GetOwningPlayerPawn());
		if (!Text.IsEmpty()) //빈 문자열이 아닌 경우 전송한다. 
		{
			//ASPCharacterPlayer Player = Cast<>(GetO);
			if (Player)
			{
				FString PlayerIdString = FString(GetOwningPlayer()->PlayerState->GetPlayerName());
				Player->ServerRPCSendMessage(PlayerIdString, Text.ToString());
				//보냈으니 비워주는 작업을 한다. 
				MessageBox->SetText(FText::GetEmpty());
			}
		}
		else
		{
			ActiveChat(false);
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetOwningPlayer());
			
		}
		// else
		// {
		// 	ASPPlayerController* PlayerController = Cast<ASPPlayerController>(GetOwningPlayer());
		// 	if(PlayerController)
		// 	{
		// 		USPHUDWidget* MyHUD = PlayerController->GetSPHUDWidget();
		// 		if(MyHUD)
		// 		{
		// 			MyHUD->ShowChat(false);
		// 		}
		// 	}
		// }
		
	}
}

void USPChatWidget::AddMessageOntoMessagesList(const FString& Sender, const FString& Message)
{
	UTextBlock* NewTextBlock = NewObject<UTextBlock>(MessagesList);
	NewTextBlock->SetAutoWrapText(true);
	FSlateFontInfo FontInfo = NewTextBlock->GetFont();
	FontInfo.Size = 17;
	NewTextBlock->SetFont(FontInfo);
	FString SenderMessageString = FString::Printf(TEXT("%s: %s"),*Sender,*Message);
	
	NewTextBlock->SetText(FText::FromString(SenderMessageString));
	
	MessagesList->AddChild(NewTextBlock);
	MessagesList->ScrollToEnd();
}

void USPChatWidget::SetFousOnChat()
{
	// bIsFocused= IsFocus;
	// if(bIsFocused)
	// {
	//FInputModeUIOnly InputMode;
	//InputMode.SetWidgetToFocus(MessageBox);
	ActiveChat(true);
	MessageBox->SetKeyboardFocus();
	// }
}

void USPChatWidget::ActiveChat(bool bIsActiveChat)
{
	if(bIsActiveChat)
	{
		MessageBox->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		MessageBox->SetVisibility(ESlateVisibility::Collapsed);
	}
}

// void USPChatWidget::ShowChat()
// {
// 	//UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetOw);
// }
