// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Trigger/SPScoreTrigger.h"

#include "SpectrumLog.h"
#include "SPGameState.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameState.h"
#include "Object/SPObject.h"
#include "Potion/SPSpectrumPotion.h"

class ASPGameModeBase;
// Sets default values
ASPScoreTrigger::ASPScoreTrigger()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	//SetReplicates(true);
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
			ServerRPC(Object);
		}
	}

	ASPSpectrumPotion* SpectrumPotionHit = Cast<ASPSpectrumPotion>(OtherActor);
	if (SpectrumPotionHit)
	{
		SetOwner(OtherActor->GetOwner());
		SP_LOG(LogSPNetwork,Log,TEXT("%s"),*(this->GetOwner())->GetName());
		
		TArray<AActor*> OverlappingActors;
		Trigger->GetOverlappingActors(OverlappingActors);

		ServerSpectrumRPC(OverlappingActors);
		
		// UE_LOG(LogTemp,Log,TEXT("%d"), OverlappingActors.Num());
		// int num = 0;
		// for (AActor* Actor : OverlappingActors)
		// {
		// 	if (ASPObject* MyObject = Cast<ASPObject>(Actor))
		// 	{
		// 		if (MyObject->bIsScoreReflected == false)
		// 		{
		// 			UE_LOG(LogTemp,Log,TEXT("ASPObject"));
		// 			//SP_LOG(LogSPNetwork,Log,TEXT("%s"),*(MyObject->GetOwner())->GetName());
		// 			ServerRPC(MyObject);
		// 		}
		// 	}
		// }
	}
}

// void ASPScoreTrigger::MultiSpectrumRPC_Implementation(ASPObject* Object)
// {
// 	if(Object->MyColorType != Color)
// 	{
// 		ISPDamageInterface* ChangeColor = Cast<ISPDamageInterface>(Object);
// 			
// 		if(Color==ColorType::Green)
// 		{
// 			ChangeColor->OnChangeColorGreen();
// 		}
// 		else if(Color==ColorType::Orange)
// 		{
// 			ChangeColor->OnChangeColorOrange();
// 		}
// 		if(Color==ColorType::Purple)
// 		{
// 			ChangeColor->OnChangeColorPurple();
// 		}
// 	}
// }

void ASPScoreTrigger::ServerSpectrumRPC_Implementation(const TArray<AActor*>& ObjectArray)
{
	for (AActor* Actor : ObjectArray)
	{
		if (ASPObject* MyObject = Cast<ASPObject>(Actor))
		{
			if (MyObject->bIsScoreReflected == false)
			{
				//UE_LOG(LogTemp,Log,TEXT("ASPObject"));
				//SP_LOG(LogSPNetwork,Log,TEXT("%s"),*(MyObject->GetOwner())->GetName());
				//ServerRPC(MyObject);
				MyObject->bIsScoreReflected = true;
				ACharacter* player = Cast<ACharacter>(GetOwner());
				if (player)
				{
					//SP_LOG(LogSPNetwork,Log,TEXT("ServerRPC2"));
					//MultiSpectrumRPC(Object);
					AGameStateBase* State = player->GetController()->GetWorld()->GetGameState();

					ISPScoreInterface* ScoreFuntion = Cast<ISPScoreInterface>(State);

					if (ScoreFuntion)
					{
						ScoreFuntion->AddScore(Color); //State에서 점수를 올려준다. 
					}
				}
			}
		}
	}
}

void ASPScoreTrigger::ServerRPC_Implementation(ASPObject* Object)
{
	SP_LOG(LogSPNetwork,Log,TEXT("ServerRPC!"));

	Object->bIsScoreReflected = true;
	ACharacter* player = Cast<ACharacter>(GetOwner());
	if (player)
	{
		SP_LOG(LogSPNetwork,Log,TEXT("ServerRPC2"));
		//MultiSpectrumRPC(Object);
		AGameStateBase* State = player->GetController()->GetWorld()->GetGameState();

		ISPScoreInterface* ScoreFuntion = Cast<ISPScoreInterface>(State);

		if (ScoreFuntion)
		{
			ScoreFuntion->AddScore(Color); //State에서 점수를 올려준다. 
		}
	}
}
