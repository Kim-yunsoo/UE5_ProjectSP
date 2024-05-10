// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Trigger/SPPortalTrigger.h"
#include "Components/BoxComponent.h"
#include "Interface/SPTriggerInterface.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASPPortal::ASPPortal()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetRelativeScale3D(FVector(8.5f,8.0f,5.75f));
	Trigger->SetBoxExtent(FVector(32.0f,32.0f,32.0f));
	Trigger->SetMobility(EComponentMobility::Static);

	
	// PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));

	static ConstructorHelpers::FObjectFinder<UDataTable> DataRef(TEXT("/Script/Engine.DataTable'/Game/Spectrum/Data/DT_PortalData.DT_PortalData'"));
	if(DataRef.Object)
	{
		PortalData=DataRef.Object;
	}

	PortalSound = CreateDefaultSubobject<USoundWave>(TEXT("PortalSound"));
	static ConstructorHelpers::FObjectFinder<USoundWave> SoundRef(TEXT("/Script/Engine.SoundWave'/Game/Spectrum/Sound/PortalSound.PortalSound'"));
	if(SoundRef.Object)
	{
		PortalSound=SoundRef.Object;
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
}

void ASPPortal::ClientRPCSound_Implementation(USoundWave* Sound)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetActorLocation(),GetActorRotation());
}

void ASPPortal::ServerRPC_Implementation(AActor* OverlapActor, const FVector& Location)
{
	ISPTriggerInterface* PortalInterface = Cast<ISPTriggerInterface>(OverlapActor);
	if(PortalInterface)
	{
		ClientRPCSound(PortalSound);
		PortalInterface->OverlapPortal(LocationResult);
	}
}

