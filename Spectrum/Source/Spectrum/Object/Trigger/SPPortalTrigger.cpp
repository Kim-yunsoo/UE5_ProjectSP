// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Trigger/SPPortalTrigger.h"

#include "NiagaraFunctionLibrary.h"
#include "SpectrumLog.h"
#include "Components/BoxComponent.h"
#include "Interface/SPTriggerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASPPortal::ASPPortal()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetRelativeScale3D(FVector(8.5f, 8.0f, 5.75f));
	Trigger->SetBoxExtent(FVector(32.0f, 32.0f, 32.0f));
	Trigger->SetMobility(EComponentMobility::Static);

	SetRootComponent(Trigger);

	// PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));

	static ConstructorHelpers::FObjectFinder<UDataTable> DataRef(
		TEXT("/Script/Engine.DataTable'/Game/Spectrum/Data/DT_PortalData.DT_PortalData'"));
	if (DataRef.Object)
	{
		PortalData = DataRef.Object;
	}

	// PortalSound = CreateDefaultSubobject<USoundWave>(TEXT("PortalSound"));
	static ConstructorHelpers::FObjectFinder<USoundWave> SoundRef(
		TEXT("/Script/Engine.SoundWave'/Game/Spectrum/Sound/PortalSound.PortalSound'"));
	if (SoundRef.Object)
	{
		PortalSound = SoundRef.Object;
	}

	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(
	TEXT("/Script/Engine.ParticleSystem'/Game/Box/InfinityBladeEffects/Effects/FX_Skill_Aura/P_AuraCircle_Ice_Base_01.P_AuraCircle_Ice_Base_01'"));
	if (EffectRef.Succeeded())
	{
		Effect = EffectRef.Object;
	}

	//
	// PortalEffect= CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MainVFX"));
	// SetRootComponent(PortalEffect);
	// static ConstructorHelpers::FObjectFinder<UParticleSystem> HealEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/Box/InfinityBladeEffects/Effects/FX_Skill_Aura/P_AuraCircle_Ice_Base_01.P_AuraCircle_Ice_Base_01'"));
	// if(HealEffectRef.Succeeded())
	// {
	// 	PortalEffect->SetTemplate(HealEffectRef.Object);
	// 	PortalEffect->SetRelativeLocation(GetActorLocation() - FVector{0.0f,0.0f,-500.0f});
	// }
}

void ASPPortal::BeginPlay()
{
	Super::BeginPlay();

	if (PortalData)
	{
		LocationData = PortalData->FindRow<FPortalDataBase>(FName(TEXT("LocationData")), "");
		LocationResult = FVector(LocationData->PortalX, LocationData->PortalY, LocationData->PortalZ);
	}

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASPPortal::OnBeginOverlap);
}

void ASPPortal::DestroyEffect(UParticleSystemComponent* ParticleComponent)
{
	if(ParticleComponent)
	{
		ParticleComponent->DestroyComponent();
	}
}

void ASPPortal::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                               int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SetOwner(OtherActor);

	ClientRPCSound(PortalSound);
	if (HasAuthority())
	{
		ServerRPC(OtherActor, LocationResult);
	}
	
}

void ASPPortal::ClientRPCSound_Implementation(USoundWave* Sound)
{
	FVector Location = GetActorLocation() - FVector {0.0f , 0.0f , 300.0f} ;
	FVector Scale = FVector{2.0f, 2.0f, 2.0f} ;
	UParticleSystemComponent* ParticleComponent= UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Effect,Location, GetActorRotation(),Scale );
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetActorLocation(), GetActorRotation());

	FTimerHandle DestroyTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, [ParticleComponent]()
		 {
			 ParticleComponent->DestroyComponent();
		 }, 4.0f, false);
}



void ASPPortal::ServerRPC_Implementation(AActor* OverlapActor, const FVector& Location)
{
	ISPTriggerInterface* PortalInterface = Cast<ISPTriggerInterface>(OverlapActor);
	if (PortalInterface)
	{
		PortalInterface->OverlapPortal(LocationResult);
	}
}
