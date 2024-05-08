// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Trigger/SPNoGrappingTrigger.h"
#include "Components/BoxComponent.h"

// Sets default values
ASPNoGrappingTrigger::ASPNoGrappingTrigger()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetRelativeLocation(FVector(-120.0f, 270.f, 5850.0f));
	Trigger->SetRelativeScale3D(FVector(89.75f, 83.25f, 16.25f));
	Trigger->SetMobility(EComponentMobility::Static);
	Trigger->SetCollisionProfileName(TEXT("NoGrapingCollision"));
}

// Called when the game starts or when spawned
void ASPNoGrappingTrigger::BeginPlay()
{
	Super::BeginPlay();

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASPNoGrappingTrigger::OnBeginOverlap);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ASPNoGrappingTrigger::OnTriggerEndOverlap);
}

void ASPNoGrappingTrigger::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp,
                                          int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// SetOwner(OtherActor);
	//
	// if (HasAuthority())
	// {
	// 	if (Cast<ASPCharacterPlayer>(OtherActor))
	// 	{
	// 		ServerRPC(OtherActor, false);
	// 	}
	// }
}

void ASPNoGrappingTrigger::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// if (HasAuthority())
	// {
	// 	if (Cast<ASPCharacterPlayer>(OtherActor))
	// 	{
	// 		ServerRPC(OtherActor, true);
	// 	}
	// }
}


void ASPNoGrappingTrigger::ServerRPC_Implementation(AActor* Other, bool Active)
{
	// ISPTriggerInterface* TriggerInterface = Cast<ISPTriggerInterface>(Other);
	// if (TriggerInterface)
	// {
	// 	TriggerInterface->ActiveGrapping(Active);
	// }
}
