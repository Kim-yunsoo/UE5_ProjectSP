// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Trigger/SPScoreTrigger.h"

#include "SpectrumLog.h"
#include "SPGameState.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameState.h"
#include "Object/SPObject.h"
#include "Potion/SPSpectrumPotion.h"

class ASPGameModeBase;
// Sets default values
ASPScoreTrigger::ASPScoreTrigger()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	//SetReplicates(true);
	bReplicates = true;
}

// Called when the game starts or when spawned
void ASPScoreTrigger::BeginPlay()
{
	Super::BeginPlay();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASPScoreTrigger::OnBeginOverlap);
}

void ASPScoreTrigger::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                     const FHitResult& SweepResult)
{
	ASPObject* Object = Cast<ASPObject>(OtherActor);
	if (Object)
	{
		SetOwner(OtherActor->GetOwner());
		if (Object->MyColorType == Color && Object->bIsScoreReflected == false)
		{
			if (HasAuthority())
			{
				ServerRPC(Object);
			}
		}
	}

	ASPSpectrumPotion* SpectrumPotionHit = Cast<ASPSpectrumPotion>(OtherActor);
	if (SpectrumPotionHit)
	{
		SetOwner(OtherActor->GetOwner());
		SP_LOG(LogSPNetwork, Log, TEXT("%s"), *(this->GetOwner())->GetName());

		TArray<AActor*> OverlappingActors;
		Trigger->GetOverlappingActors(OverlappingActors);

		MultiSpectrumRPC(OverlappingActors);
	}
}


void ASPScoreTrigger::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ASPScoreTrigger::MultiSpectrumRPC_Implementation(const TArray<AActor*>& ObjectArray) //스펙트럼 물약들 .. 
{
	for (AActor* Actor : ObjectArray)
	{
		//UE_LOG(LogTemp,Log,TEXT("%s"),*Actor->GetName());
		if (ASPObject* MyObject = Cast<ASPObject>(Actor))
		{
			SP_LOG(LogSPNetwork, Log, TEXT("Server"));
			if (MyObject->MyColorType != Color)
			{
				ISPDamageInterface* ChangeColor = Cast<ISPDamageInterface>(MyObject);

				if (Color == ColorType::Green)
				{
					ChangeColor->OnChangeColorGreen();
				}
				else if (Color == ColorType::Orange)
				{
					ChangeColor->OnChangeColorOrange();
				}
				if (Color == ColorType::Purple)
				{
					ChangeColor->OnChangeColorPurple();
				}
			}
			//MultiSpectrumRPC(MyObject);
			if (HasAuthority())
			{
				if (MyObject->bIsScoreReflected == false)
				{
					MyObject->bIsScoreReflected = true;
					// ACharacter* player = Cast<ACharacter>(GetOwner());
					// if (player)
					// {
						AGameStateBase* State  = GetWorld()->GetGameState();
						//AGameStateBase* State = player->GetController()->GetWorld()->GetGameState();

						ISPScoreInterface* ScoreFuntion = Cast<ISPScoreInterface>(State);

						if (ScoreFuntion)
						{
							ScoreFuntion->AddScore(Color); //State에서 점수를 올려준다. 
						}
					// }
				}
			}
		}
	}
}

void ASPScoreTrigger::ServerRPC_Implementation(ASPObject* Object)
{
	Object->bIsScoreReflected = true;
	ACharacter* player = Cast<ACharacter>(GetOwner());
	if (player)
	{
		//MultiSpectrumRPC(Object);
		AGameStateBase* State = player->GetController()->GetWorld()->GetGameState();

		ISPScoreInterface* ScoreFuntion = Cast<ISPScoreInterface>(State);

		if (ScoreFuntion)
		{
			ScoreFuntion->AddScore(Color); //State에서 점수를 올려준다. 
		}
	}
}
