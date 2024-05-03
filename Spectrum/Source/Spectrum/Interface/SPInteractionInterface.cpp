// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/SPInteractionInterface.h"
#include "Character/SPCharacterPlayer.h"
#include "UI/SPHUDWidget.h"

// Add default functionality here for any ISPInteractionInterface functions that are not pure virtual.
void ISPInteractionInterface::BeginFocus()
{
}

void ISPInteractionInterface::EndFocus()
{
}

void ISPInteractionInterface::BeginInteract()
{
}

void ISPInteractionInterface::EndInteract()
{
}

void ISPInteractionInterface::Interact(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget)
{
}

void ISPInteractionInterface::Interact2(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget)
{
}
