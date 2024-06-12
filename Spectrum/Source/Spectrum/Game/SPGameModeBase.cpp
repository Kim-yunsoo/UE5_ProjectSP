// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SPGameModeBase.h"

#include "SpectrumLog.h"
#include "SPPlayerState.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Game/SPGameState.h"
#include "GameFramework/GameStateBase.h"
#include "Character/SPCharacterPlayer.h"
#include "Player/SPPlayerController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "UI/ReturnMain/SPReturnToMainMenu.h"



ASPGameModeBase::ASPGameModeBase()
{
	bDelayedStart = true;
	bUseSeamlessTravel = true;
	GameStateClass = ASPGameState::StaticClass();
	PlayerStateClass = ASPPlayerState::StaticClass();
	PlayerControllerClass= ASPPlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn> AIPawnClassRef(TEXT("/Script/Engine.Blueprint'/Game/ESM_NoviceSorceress/AI/Blueprint/BP_SPCharacterNonPlayer.BP_SPCharacterNonPlayer_C'"));
	if(AIPawnClassRef.Class)
	{
		AIPawnClass = AIPawnClassRef.Class;
	}
	
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(
		TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BT_SP_Character.BT_SP_Character'"));
	if (nullptr != BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}

	// static ConstructorHelpers::FClassFinder<USPHUDWidget> SPHUDWidgetRef(TEXT("/Game/Spectrum/UMG/WBP_SPHUD.WBP_SPHUD_C"));
	// if (SPHUDWidgetRef.Class)
	// {
	// 	SPHUDWidgetClass = SPHUDWidgetRef.Class;
	// }


}
void ASPGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	LevelStartingTime = GetWorld() ->GetTimeSeconds(); //여기까지 들어오게 된 시간 
}
void ASPGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(MatchState == MatchState::WaitingToStart)
	{
		CountdownTime=WarmupTime-GetWorld()->GetTimeSeconds()+LevelStartingTime; //10초 로딩 시간
	
		if(CountdownTime<=0.f)
		{
			StartMatch(); //진행 모드로 변환
		}
	}
}

void ASPGameModeBase::OnMatchStateSet()
{
	Super::OnMatchStateSet();
	
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ASPPlayerController* MyPlayer = Cast<ASPPlayerController>(It->Get());
		if(MyPlayer)
		{
			MyPlayer->ClientRCPMathState(MatchState);
		}
	}
	ASPGameState* SPGameState = Cast<ASPGameState>(GetWorld()->GetGameState());
	if (SPGameState)
	{
		SPGameState->OnMathStateSet(MatchState); //시간 제대로 작동 확인 
	}
	if(MatchState == MatchState::WaitingPostMatch)
	{
		//각자 위젯에 엔드 위젯 넣기 
		//SP_LOG(LogSPNetwork,Log,TEXT("WaitingPostMatch"));
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* PlayerController = Iterator->Get();
			ASPPlayerController* SPPlayerController = Cast<ASPPlayerController>(PlayerController);
			if(SPPlayerController)
			{
				SPPlayerController->ShowReturnToMainMenu();
				 //여기서 위젯 호출 
			}
		}
	}
}


void ASPGameModeBase::FinishMatch()
{
	UE_LOG(LogTemp,Log,TEXT("FinishMatch"));
	ASPGameModeBase* const SPGameState = Cast<ASPGameModeBase>(GameState);
	if(SPGameState && IsMatchInProgress())
	{
		EndMatch();
	}
}

void ASPGameModeBase::AISpawn()
{
	//AAIController* AIController = Cast<ASPCharacterNonPlayer>(AIPawnClass)->GetController()
	FVector Location= FVector{-700,330,3784};
	UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(),AIPawnClass,BTAsset,Location,FRotator::ZeroRotator);
}

void ASPGameModeBase::HandleSeamlessTravelPlayer(AController*& C)
{
	Super::HandleSeamlessTravelPlayer(C);

	if(!C)
	{
		return;
	}

	if(ASPPlayerController* MyController = Cast<ASPPlayerController>(C))
	{
		if(ASPPlayerState* PlayerState = MyController->GetPlayerState<ASPPlayerState>())
		{
			if(PlayerState)
			{
				SpawnPlayerCharacter(MyController, PlayerState->Color,PlayerState->Gender);
			}
			
		}
	}
}

void ASPGameModeBase::SpawnPlayerCharacter(APlayerController* MyController,  ColorType& MyColor,
	 GenderType& MyGender)
{
	//임시지정
	// MyColor=ColorType::Green;
	// MyGender=GenderType::Man;
	//
	FString SpawnPath;
	if(MyColor ==ColorType::Green && MyGender == GenderType::Man)
	{
		SpawnPath= TEXT("/Game/Spectrum/BluePrint/BP_SPCharacterMan2.BP_SPCharacterMan2_C");
	}
	else if(MyColor ==ColorType::Orange && MyGender == GenderType::Man)
	{
		SpawnPath = TEXT("/Game/Spectrum/BluePrint/BP_SPCharacterMan3.BP_SPCharacterMan3_C");
	}
	else if(MyColor ==ColorType::Purple && MyGender == GenderType::Man)
	{
		SpawnPath =  TEXT("/Game/Spectrum/BluePrint/BP_SPCharacterMan1.BP_SPCharacterMan1_C");
	}
	
	else if(MyColor ==ColorType::Green && MyGender == GenderType::Woman)
	{
		SpawnPath =  TEXT("/Game/Spectrum/BluePrint/BP_SPCharaterPlayer_W2.BP_SPCharaterPlayer_W2_C");
	}
	else if(MyColor ==ColorType::Orange && MyGender == GenderType::Woman)
	{
		SpawnPath = TEXT("/Game/Spectrum/BluePrint/BP_SPCharaterPlayer_W3.BP_SPCharaterPlayer_W3_C");
	}
	else if(MyColor ==ColorType::Purple && MyGender == GenderType::Woman)
	{
		SpawnPath = TEXT("/Game/Spectrum/BluePrint/BP_SPCharaterPlayer_W1.BP_SPCharaterPlayer_W1_C");
	}
	
	UClass* DesiredPawnClass = StaticLoadClass(UObject::StaticClass(), nullptr, *SpawnPath);

	if(DesiredPawnClass)
	{
		APawn* OldPawn = MyController->GetPawn();
		if(OldPawn)
		{
			OldPawn->Destroy();
		}

		FVector SpawnLocation = FVector(2547.0f, 247.0f, 2894.0f);
		// 	// 새 폰을 스폰하고 플레이어에게 할당
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = MyController;
		APawn* NewPawn = GetWorld()->SpawnActor<APawn>(DesiredPawnClass, SpawnLocation, FRotator::ZeroRotator,
		 	                                               SpawnParams);
		ASPPlayerController* PC = Cast<ASPPlayerController>(MyController);
		if(PC)
		{
			PC->Possess(NewPawn);
		}
	}
}

void ASPGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void ASPGameModeBase::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();
}


void ASPGameModeBase::SendMessagesToEveryOne(const FString& Sender, const FString& Message)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController)
		{
			//APawn* Pawn =PlayerController->GetPawn();
			//체크
			ASPCharacterPlayer* MyPlayer = Cast<ASPCharacterPlayer>(PlayerController->GetPawn());
			if (MyPlayer)
			{
				MyPlayer->ClientRPCAddMessageToChat(Sender, Message);
			}
		}
	}
}

