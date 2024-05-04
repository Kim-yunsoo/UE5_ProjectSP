// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Trigger/SPScoreTrigger.h"

#include "SPGameState.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameState.h"
#include "Object/SPObject.h"

class ASPGameModeBase;
// Sets default values
ASPScoreTrigger::ASPScoreTrigger()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
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
}

void ASPScoreTrigger::ServerRPC_Implementation(ASPObject* Object)
{
	Object->bIsScoreReflected =true;
	ACharacter* player = Cast<ACharacter>(GetOwner());
	if (player)
	{
		AGameStateBase* State = player->GetController()->GetWorld()->GetGameState();

		ISPScoreInterface* ScoreFuntion = Cast<ISPScoreInterface>(State);

		if(ScoreFuntion)
		{
			ScoreFuntion->AddScore(Color); //State에서 점수를 올려준다. 
		}
	}
}
