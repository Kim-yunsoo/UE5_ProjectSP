// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPMainMenu.h"

#include "Character/SPCharacterPlayer.h"

void USPMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void USPMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<ASPCharacterPlayer>(GetOwningPlayerPawn());
}

bool USPMainMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	//FDragDropEvent �巡�׸� �� �� �����Ͱ� ������

	//cast operation to item drag drop, ensure player is valid, call drop item on player
}
