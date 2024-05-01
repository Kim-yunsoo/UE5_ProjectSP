// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Portal/SPPortal.h"

#include "SpectrumLog.h"
#include "Character/SPCharacterPlayer.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Interface/SPPortalInterface.h"

// Sets default values
ASPPortal::ASPPortal()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	// PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));

	static ConstructorHelpers::FObjectFinder<UDataTable> DataRef(TEXT("/Script/Engine.DataTable'/Game/Spectrum/Data/DT_PortalData.DT_PortalData'"));
	if(DataRef.Object)
	{
		PortalData=DataRef.Object;
	}
}

void ASPPortal::BeginPlay()
{
	Super::BeginPlay();

	if(PortalData)
	{
		LocationData = PortalData->FindRow<FPortalDataBase>(FName(TEXT("LocationData")),"");
		LocationResult  =FVector(LocationData->PortalX,LocationData->PortalY,LocationData->PortalZ);
	}
	
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASPPortal::OnBeginOverlap);
}

void ASPPortal::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                               int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SetOwner(OtherActor);
	if (HasAuthority())
	{
		ServerRPC(OtherActor,LocationResult);
	}
		// ISPPortalInterface* PortalInterface = Cast<ISPPortalInterface>(OtherActor);
		// if(PortalInterface)
		// {
		// 	PortalInterface->OverlapPortal(LocationResult);
		// }
		// MultiRPC(OtherActor,LocationResult);
		// SP_LOG(LogSPNetwork, Log, TEXT("OverLap %s"), *GetOwner()->GetName());
	// }
}

void ASPPortal::ServerRPC_Implementation(AActor* OverlapActor, const FVector& Location)
{
	ISPPortalInterface* PortalInterface = Cast<ISPPortalInterface>(OverlapActor);
	if(PortalInterface)
	{
		PortalInterface->OverlapPortal(LocationResult);
	}
}

void ASPPortal::MultiRPC_Implementation(AActor* OverlapActor,const FVector& Location)
{
	ISPPortalInterface* PortalInterface = Cast<ISPPortalInterface>(OverlapActor);
	if(PortalInterface)
	{
		PortalInterface->OverlapPortal(Location);
	}
	
}
