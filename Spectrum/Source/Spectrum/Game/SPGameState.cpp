// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SPGameState.h"

#include "SpectrumLog.h"
#include "Data/SPSpawnPosition.h"
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

	RemainingTime = MatchPlayTime;

	static ConstructorHelpers::FObjectFinder<UDataTable> TableRef(TEXT("/Script/Engine.DataTable'/Game/Spectrum/ItemData/DT_SpawnPosition.DT_SpawnPosition'"));
	if(TableRef.Object)
	{
		PositionTable = TableRef.Object;
	}
	
}

void ASPGameState::BeginPlay()
{
	Super::BeginPlay();
		GetWorldTimerManager().SetTimer(GameTimerHandle, this, &ASPGameState::DefaultGameTimer,
		                                GetWorldSettings()->GetEffectiveTimeDilation(), true);
}

void ASPGameState::AddScore(const ColorType& MyColor)
{
	if (MyColor == ColorType::Green)
	{
		if(HasAuthority())
		{
		++GreenScore;
		On_RapGreenScore();
		}
	}
	else if (MyColor == ColorType::Orange)
	{
		if(HasAuthority())
		{
			++OrangeScore;
			On_RapOrangeScore();
		}
	}
	else if (MyColor == ColorType::Purple)
	{
		if(HasAuthority())
		{
			++PurpleScore;
			On_RapPurpleScore();
		}
	}
}

void ASPGameState::DefaultGameTimer()
{
	if(HasAuthority())
	{
		if (RemainingTime > 0)
		{
			RemainingTime--;
			OnRapTime();
			if(RemainingTime == SpectrumPotionSpawnTime) //현재 3분이라면? 
			{
				SpectrumPotionSpawn(); //물약 스폰 
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

void ASPGameState::Ready()
{
	++ReadyCount;
	if(ReadyCount==2)
	{
		GetWorld()->ServerTravel(TEXT("/Game/Spectrum/Room/Map/Building?listen"));
	}
}
void ASPGameState::MoveToInGame()
{
	GetWorld()->ServerTravel(TEXT("/Game/Spectrum/Room/Map/Building?listen"));
}

void ASPGameState::SpectrumPotionSpawn()
{
	UE_LOG(LogTemp,Log,TEXT("SpectrumPotionSpawn"));
	//GetWorld()->SpawnActorDeferred<ASPPickup>()
	TArray<FName> RowNames = PositionTable->GetRowNames();
	int32 RandomIndex = FMath::RandRange(0,PositionTable->GetRowNames().Num()-1); // 인덱스를 뽑아야하니까 -1
	

	if(RandomIndex>=0)
	{
		FName RandomRowName = RowNames[RandomIndex];
		FPosition* RandomPosition = PositionTable->FindRow<FPosition>(RandomRowName,TEXT(""));
		//GetWorld()->SpawnActorDeferred<ASPPickup>( );

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
		FTransform SpawnTransform(FRotator::ZeroRotator, RandomPosition->Position);
		ASPPickup* MyActor =GetWorld()->SpawnActorDeferred<ASPPickup>(ASPPickup::StaticClass(),SpawnTransform);
		//MyActor->SetOwner(Owner);
		MyActor->bIsSpectrumPotion = true;
		//SP_SUBLOG(LogSPNetwork,Log,TEXT("Owner"));
		MyActor->FinishSpawning(SpawnTransform);
		
	}
	//Getplayer
	//
	//SP_LOG(LogSPNetwork,Log,TEXT(" Owner Name ?? : %s"), *GetOwner()->GetName())
	//GetWorld()->GetPlayerControllerIterator()
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

void ASPGameState::MultiRPCSpawnUI_Implementation()
{
	
}

void ASPGameState::ServerRPC_Implementation()
{
	//SP_LOG(LogSPNetwork,Log,TEXT("In Travel22!!"));
	GetWorld()->ServerTravel(TEXT("/Game/Spectrum/Room/Map/Building?listen"));

}


void ASPGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPGameState, GreenScore);
	DOREPLIFETIME(ASPGameState, OrangeScore);
	DOREPLIFETIME(ASPGameState, PurpleScore);
	DOREPLIFETIME(ASPGameState, RemainingTime);
	DOREPLIFETIME(ASPGameState, ReadyCount);
}


