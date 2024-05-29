// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SPPlayerController.h"
#include "UI/SPHUDWidget.h"
#include "SpectrumGameInstance.h"
#include "SpectrumLog.h"
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

   //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("OnPossess"));
   auto Instance = Cast<USpectrumGameInstance>(GetWorld()->GetGameInstance());
   if (!Instance) return;
   if (Instance->ClientPawnClass == nullptr) return;

   GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, 
      UKismetSystemLibrary::GetClassDisplayName(Instance->ClientPawnClass));
   if (Instance->bAlreadyChangePawn) return;

   Instance->bAlreadyChangePawn = true;
//   CallServerDuetoChangePawn(aPawn->GetActorLocation(), Instance->ClientPawnClass, aPawn);
}

// void ASPPlayerController::ChangePawnName(FString path)
// {
//    FString BPPath = path;
//    UBlueprint* BPClass = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), nullptr, *BPPath));
//    if (!BPClass) return;
//
//    UClass* PawnClass = BPClass->GeneratedClass;
//    if (!PawnClass) return;
//
//    TSubclassOf<APawn> PAWN_C = PawnClass;
//    //CallServerDuetoChangePawn(GetPawn()->GetActorLocation(), PAWN_C, GetPawn());
// }


// void ASPPlayerController::CallServerDuetoChangePawn_Implementation(FVector location, TSubclassOf<APawn> PAWN_C, APawn* OLDpAWN)
// {
//    auto mode = Cast<ASPGameModeBase>(GetWorld()->GetAuthGameMode());
//    if (!mode) return;
//
//    if (mode->TryChangePawn(this, location, PAWN_C))
//       mode->TryDestroyOldpawn(OLDpAWN);
// }

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

USPHUDWidget* ASPPlayerController::GetSPHUDWidget() const
{
   return SPHUDWidget;
}

void ASPPlayerController::SetHUDMatchCountdown(float CountdownTime)
{
   // SPHUDWidget* SPHUD = SPHUD== nullptr? Cast<USPHUDWidget>(GetHUD()) : SPHUD; 
   // int32 Minutes = FMath::FloorToInt(CountdownTime/60.f);
   //
   // int32 Seconds = CountdownTime - Minutes *60;
   //
   // FString CountdownText = FString::Printf(TEXT("%02d:%02d"),Minutes,Seconds);
   // // if(MatchCountDownText)
   // // {
   // //    MatchCountDownText->SetText(FText::FromString(CountdownText));
   // // }
}

void ASPPlayerController::ClientRPCSpawnUI_Implementation(const int32 Index)
{
   SPHUDWidget->UpdatePotionUI(Index);
}

