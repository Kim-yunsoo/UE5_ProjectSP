// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SPPlayerController.h"
#include "UI/SPHUDWidget.h"
#include "SpectrumGameInstance.h"
#include "SPGameModeBase.h"
#include "Net/UnrealNetwork.h"

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

   //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("OnPossess"));
   auto Instance = Cast<USpectrumGameInstance>(GetWorld()->GetGameInstance());
   if (!Instance) return;
   if (Instance->ClientPawnClass == nullptr) return;

   GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, 
      UKismetSystemLibrary::GetClassDisplayName(Instance->ClientPawnClass));
   if (Instance->bAlreadyChangePawn) return;

   Instance->bAlreadyChangePawn = true;

}



void ASPPlayerController::BeginPlay()
{
   Super::BeginPlay();

   FInputModeGameOnly GameOblyInputMode;
   SetInputMode(GameOblyInputMode);

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

void ASPPlayerController::OnMathStateSet(FName State)
{
   MatchState = State;
   if(MatchState == MatchState::InProgress)
   {
     
   }
}

void ASPPlayerController::OnRep_MatchState()
{
}

void ASPPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
   Super::GetLifetimeReplicatedProps(OutLifetimeProps);
   DOREPLIFETIME(ASPPlayerController, MatchState);
}

USPHUDWidget* ASPPlayerController::GetSPHUDWidget() const
{
   return SPHUDWidget;
}



void ASPPlayerController::ClientRPCSpawnUI_Implementation(const int32 Index)
{
   SPHUDWidget->UpdatePotionUI(Index);
}

