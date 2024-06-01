// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SPPlayerController.h"
#include "UI/SPHUDWidget.h"
#include "SPGameModeBase.h"

ASPPlayerController::ASPPlayerController()
{
   static ConstructorHelpers::FClassFinder<USPHUDWidget> SPHUDWidgetRef(TEXT("/Game/Spectrum/UMG/WBP_SPHUD.WBP_SPHUD_C"));
   if (SPHUDWidgetRef.Class)
   {
      SPHUDWidgetClass = SPHUDWidgetRef.Class;
   }
 
}

void ASPPlayerController::OnPossess(APawn* aPawn)
{
   Super::OnPossess(aPawn);
}



void ASPPlayerController::BeginPlay()
{
   Super::BeginPlay();

   FInputModeGameOnly GameOnlyInputMode;
   SetInputMode(GameOnlyInputMode);
   if(IsLocalPlayerController())
   {
      SPHUDWidget = CreateWidget<USPHUDWidget>(this, SPHUDWidgetClass);
      if (SPHUDWidget)
      {
         SPHUDWidget->AddToViewport();
         SPHUDWidget->SetVisibility(ESlateVisibility::Visible);

      }
   }


}

void ASPPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
   Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

USPHUDWidget* ASPPlayerController::GetSPHUDWidget() const
{
   return SPHUDWidget;
}

void ASPPlayerController::ClientRCPMathState_Implementation(FName State)
{
   if(State == MatchState::InProgress)
   {
    
      if(SPHUDWidget)
      {
          SPHUDWidget->HideLoadingWidget();
      }
      
   }
}


void ASPPlayerController::ClientRPCSpawnUI_Implementation(const int32 Index)
{
   SPHUDWidget->UpdatePotionUI(Index);
}

