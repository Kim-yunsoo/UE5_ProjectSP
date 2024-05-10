// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SPOrangeExplosionComponent.h"
#include "Particles/ParticleSystem.h"
#include "Interface/SPDamageInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

USPOrangeExplosionComponent::USPOrangeExplosionComponent()
{
	WaterSound = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Spectrum/Sound/Water2.Water2'"));
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitRef(
TEXT("/Script/Engine.ParticleSystem'/Game/MagicProjectilesVol2/Particles/Hits/CP_OrangePotion.CP_OrangePotion'"));

	if (HitRef.Succeeded())
	{
		EmitterHit = HitRef.Object;
	}
}

void USPOrangeExplosionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USPOrangeExplosionComponent::Explode()
{
	Super::Explode();
	//Multi Sphere Trace For Object
	//Start & End
	FVector SphereTracePoint = GetOwner()->GetRootComponent()->GetComponentLocation();
	float Radius = 125.f;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> OutHits;
	FLinearColor GreenColor(0.0f, 1.0f, 0.0f);
	FLinearColor RedColor(1.0f, 0.0f, 0.0f);
	float DrawTime = 5.0f;
	bool Success = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), SphereTracePoint, SphereTracePoint,
																	Radius, ObjectTypes, false, ActorsToIgnore,
																	EDrawDebugTrace::None, OutHits, true,
																	GreenColor, RedColor, DrawTime);
	if (Success)
	{
		MultiRPCOrangeExplosion(OutHits);
	}
}

void USPOrangeExplosionComponent::MultiRPCOrangeExplosion_Implementation(const TArray<FHitResult>& Array)
{
	for (const FHitResult& HitResult : Array)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			ActorArray.AddUnique(HitActor);
		}
	}

	if (ActorArray.Num() > 0)
	{
		for (AActor*& HitActor : ActorArray)
		{
			ISPDamageInterface* DamageInterface = Cast<ISPDamageInterface>(HitActor);
			if (DamageInterface)
			{
				DamageInterface->OnChangeColorOrange();
			}
		}
	}
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterHit, GetOwner()->GetActorLocation(), FRotator::ZeroRotator,
												 FVector(1.0f), true, EPSCPoolMethod::None, true);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), WaterSound, GetOwner()->GetActorLocation());
}
