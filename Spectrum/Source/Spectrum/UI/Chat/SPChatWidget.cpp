// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Chat/SPChatWidget.h"

#include "Character/SPCharacterPlayer.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void USPChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MessagesList = Cast<UScrollBox>(GetWidgetFromName(TEXT("MessagesList")));
	MessageBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("MessageBox")));

	//델리게이트 연결 
	MessageBox->OnTextCommitted.AddDynamic(this, &USPChatWidget::OnChatTextCommitted);
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
				FString PlayerIdString = FString::FromInt(GetOwningPlayer()->PlayerState->GetPlayerId());
				Player->ServerRPCSendMessage(PlayerIdString, Text.ToString());
				//보냈으니 비워주는 작업을 한다. 
				MessageBox->SetText(FText::GetEmpty());
			}
		}
		
	}
}

void USPChatWidget::AddMessageOntoMessagesList(const FString& Sender, const FString& Message)
{
	UTextBlock* NewTextBlock = NewObject<UTextBlock>(MessagesList);
	FString SenderMessageString = FString::Printf(TEXT("%s: %s"),*Sender,*Message);
	NewTextBlock->SetText(FText::FromString(SenderMessageString));
	MessagesList->AddChild(NewTextBlock);
	MessagesList->ScrollToEnd();
}

void USPChatWidget::SetFousOnChat(bool IsFocus)
{
	if(IsFocus)
	{
		MessageBox->SetKeyboardFocus();
	}
}

// void USPChatWidget::ShowChat()
// {
// 	//UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetOw);
// }
