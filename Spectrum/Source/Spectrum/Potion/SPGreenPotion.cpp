// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion/SPGreenPotion.h"
#include "Component/SPGreenExplosionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"


ASPGreenPotion::ASPGreenPotion()
{
	SphereComponent->SetSphereRadius(35.46f);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PotionMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Spectrum/Assets/Potion/G_Potion/G_Potion1.G_Potion1'"));
	if (PotionMeshRef.Object)
	{
		PotionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GreenPotionMesh"));
		PotionMesh->SetStaticMesh(PotionMeshRef.Object);
		PotionMesh->SetupAttachment(SphereComponent);
		PotionMesh->SetRelativeLocation(FVector(2.98f, 0.74f, -20.10f));
		PotionMesh->SetRelativeScale3D(FVector(0.025f, 0.025f, 0.025f));
		PotionMesh->SetCollisionProfileName(TEXT("NoCollision"));
	}


	GreenExplosionComponent = CreateDefaultSubobject<USPGreenExplosionComponent>(TEXT("ExplosionComponent"));
	WaterSound = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Spectrum/Sound/Water2.Water2'"));


	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitRef(
	TEXT("/Script/Engine.ParticleSystem'/Game/MagicProjectilesVol2/Particles/Hits/CP_GreenPotion.CP_GreenPotion'"));

	if (HitRef.Succeeded())
	{
		EmitterHit = HitRef.Object;
	}

	
}

void ASPGreenPotion::BeginPlay()
{
	Super::BeginPlay();
	OnActorHit.AddDynamic(this, &ASPGreenPotion::HandleActorHit);
	if(HasAuthority())
	{
		this->SetReplicates(true);
		this->AActor::SetReplicateMovement(true);
		GreenExplosionComponent->SetIsReplicated(true);
	}
}

void ASPGreenPotion::HandleActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{

	UGameplayStatics::PlaySound2D(GetWorld(), WaterSound);
	FVector HitLocation = Hit.ImpactPoint;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterHit, HitLocation, FRotator::ZeroRotator,
												 FVector(1.0f), true, EPSCPoolMethod::None, true);
	GreenExplosionComponent->Explode();
	this->SetLifeSpan(0.1f);
}
