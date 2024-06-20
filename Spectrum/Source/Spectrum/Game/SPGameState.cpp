// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SPGameState.h"

#include "SpectrumLog.h"
#include "SPGameModeBase.h"
#include "Data/SPSpawnPosition.h"
#include "Enums/SPScoreType.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/SPPlayerController.h"
#include "Potion/SPPickup.h"

struct FPosition;
class ASPScoreTrigger;

ASPGameState::ASPGameState()
{
	GreenScore = 0;
	OrangeScore = 0;
	PurpleScore = 0;
	ReadyCount= 0;
	bIsInGame=false;

	RemainingTime = MatchPlayTime;

	static ConstructorHelpers::FObjectFinder<UDataTable> TableRef(TEXT("/Script/Engine.DataTable'/Game/Spectrum/ItemData/DT_SpawnPosition.DT_SpawnPosition'"));
	if(TableRef.Object)
	{
		PositionTable = TableRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundWave> SoundRef(TEXT("/Game/Spectrum/Sound/A_Winter_Migration"));
	if(SoundRef.Object)
	{
		BackGroundMusic = SoundRef.Object;
	}
}

void ASPGameState::BeginPlay()
{
	Super::BeginPlay();
	// GetWorldTimerManager().SetTimer(GameTimerHandle, this, &ASPGameState::DefaultGameTimer,
	// 									GetWorldSettings()->GetEffectiveTimeDilation(), true);
}

void ASPGameState::AddScore(const ColorType& MyColor,EScoreType ScoreType)
{
	int32 ScoreUpPoint = 0; 
	if(ScoreType == EScoreType::Default )
	{
		ScoreUpPoint = 1;
	}
	else if(ScoreType == EScoreType::AttackDefault )
	{
		ScoreUpPoint= 2; 
	}
	else if(ScoreType == EScoreType::AttackSpecial )
	{
		ScoreUpPoint= 4; 
	}
	
	if (MyColor == ColorType::Green)
	{
		if(HasAuthority())
		{
		GreenScore+=ScoreUpPoint;
		On_RapGreenScore();
		}
	}
	else if (MyColor == ColorType::Orange)
	{
		if(HasAuthority())
		{
			OrangeScore+=ScoreUpPoint;
			On_RapOrangeScore();
		}
	}
	else if (MyColor == ColorType::Purple)
	{
		if(HasAuthority())
		{
			PurpleScore+=ScoreUpPoint;
			On_RapPurpleScore();
		}
	}
}

void ASPGameState::DefaultGameTimer()
{
	if(HasAuthority())
	{
		ASPGameModeBase* GameMode = Cast<ASPGameModeBase>(GetWorld()->GetAuthGameMode());

		if (RemainingTime > 0)
		{
			RemainingTime--;
			OnRapTime();
			
			if(RemainingTime == SpectrumPotionSpawnTime) //현재 3분이라면? 
			{
				SpectrumPotionSpawn(); //물약 스폰 
			}
			
			if(RemainingTime == AISpawnTime)
			{
				AIAlarmUI();
				GameMode->AISpawn();
				
			}
			if(RemainingTime<=0) 
			{
				
				GameMode->EndMatch();
			}
		}
	}
}

void ASPGameState::On_RapGreenScore()
{
	OnScore.Broadcast(ColorType::Green, GreenScore);
}

void ASPGameState::On_RapOrangeScore()
{
	OnScore.Broadcast(ColorType::Orange, OrangeScore);
}

void ASPGameState::On_RapPurpleScore()
{
	OnScore.Broadcast(ColorType::Purple, PurpleScore);
}
void ASPGameState::OnRapTime()
{
	OnTime.Broadcast(RemainingTime);
}

void ASPGameState::OnInGamePlaySound()
{
	UGameplayStatics::PlaySound2D(this, BackGroundMusic);
}

void ASPGameState::Ready()
{
	++ReadyCount;
	if(ReadyCount==2)
	{
		GetWorld()->ServerTravel(TEXT("/Game/Spectrum/Room/Map/Building?listen"));
	}
}


void ASPGameState::SpectrumPotionSpawn()
{
	TArray<FName> RowNames = PositionTable->GetRowNames();
	int32 RandomIndex = FMath::RandRange(0,PositionTable->GetRowNames().Num()-1); // 인덱스를 뽑아야하니까 -1

	if(RandomIndex>=0)
	{
		FName RandomRowName = RowNames[RandomIndex];
		FPosition* RandomPosition = PositionTable->FindRow<FPosition>(RandomRowName,TEXT(""));
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
		FTransform SpawnTransform(FRotator::ZeroRotator, RandomPosition->Position);
		ASPPickup* MyActor =GetWorld()->SpawnActorDeferred<ASPPickup>(ASPPickup::StaticClass(),SpawnTransform);
		MyActor->bIsSpectrumPotion = true;
		MyActor->FinishSpawning(SpawnTransform);
	}
	
	for(FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It;++It)
	{
		APlayerController* PlayerController = It->Get();
		ASPPlayerController* MyPlayer = Cast<ASPPlayerController>(PlayerController);
		if(MyPlayer)
		{
			MyPlayer->ClientRPCSpawnUI(RandomIndex);
		}
	}
}

void ASPGameState::AIAlarmUI()
{
	for(FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It;++It)
	{
		APlayerController* PlayerController = It->Get();
		ASPPlayerController* MyPlayer = Cast<ASPPlayerController>(PlayerController);
		if(MyPlayer)
		{
			MyPlayer->ClientAIAlarmUI();
			//MyPlayer->ClientRPCSpawnUI(RandomIndex);
		}
	}
}

void ASPGameState::StartTimer()
{
	GetWorldTimerManager().SetTimer(GameTimerHandle, this, &ASPGameState::DefaultGameTimer,
										GetWorldSettings()->GetEffectiveTimeDilation(), true);
}

void ASPGameState::OnMathStateSet(FName State) //서버
{
	if(State ==  MatchState::InProgress)
	{
		StartTimer();
		bIsInGame=true;
		OnInGamePlaySound();
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* PlayerController = Iterator->Get();
			if (PlayerController)
			{
				ASPCharacterPlayer* MyCharacter = Cast<ASPCharacterPlayer>(PlayerController->GetCharacter());
				if (MyCharacter)
				{
					MyCharacter->bCanUseInput=true;
				}
			}
		}
	}
	if(State == MatchState::WaitingPostMatch  )
	{
		TArray<FColorScoreData> ColorScoreArray;
		ColorScoreArray.Add(FColorScoreData(ColorType::Green,GreenScore));
		ColorScoreArray.Add(FColorScoreData(ColorType::Orange,OrangeScore));
		ColorScoreArray.Add(FColorScoreData(ColorType::Purple,PurpleScore));
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* PlayerController = Iterator->Get();
			ASPPlayerController* SPPlayerController = Cast<ASPPlayerController>(PlayerController);
			if (SPPlayerController)
			{
				SPPlayerController->ClientRPCReturnToMainMenu(ColorScoreArray); //ClientRPC인데 
			}
		}
	}
}



void ASPGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPGameState, GreenScore);
	DOREPLIFETIME(ASPGameState, OrangeScore);
	DOREPLIFETIME(ASPGameState, PurpleScore);
	DOREPLIFETIME(ASPGameState, RemainingTime);
	DOREPLIFETIME(ASPGameState, ReadyCount);
	DOREPLIFETIME(ASPGameState, bIsInGame);
}


