// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/SPGreenExplosionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/SPDamageInterface.h"
#include "Particles/ParticleSystem.h"

// Sets default values for this component's properties
USPGreenExplosionComponent::USPGreenExplosionComponent()
{
	WaterSound = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Spectrum/Sound/Water2.Water2'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitRef(
TEXT("/Script/Engine.ParticleSystem'/Game/MagicProjectilesVol2/Particles/Hits/CP_GreenPotion.CP_GreenPotion'"));

	if (HitRef.Succeeded())
	{
		EmitterHit = HitRef.Object;
	}
}


// Called when the game starts
void USPGreenExplosionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USPGreenExplosionComponent::Explode(ColorType& MyColor)
{
	Super::Explode(MyColor);
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
		MultiRPCGreenExplosion(OutHits);
	}
}

void USPGreenExplosionComponent::MultiRPCGreenExplosion_Implementation(const TArray<FHitResult>& OutHits)
{

	for (const FHitResult& HitResult : OutHits)
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
				DamageInterface->OnChangeColorGreen();
			}
		}
	}
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), WaterSound, GetOwner()->GetActorLocation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterHit, GetOwner()->GetActorLocation(), FRotator::ZeroRotator,
											 FVector(1.0f), true, EPSCPoolMethod::None, true);
}




