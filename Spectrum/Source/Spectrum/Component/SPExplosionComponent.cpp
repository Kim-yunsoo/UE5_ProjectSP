// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SPExplosionComponent.h"

#include "SpectrumLog.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Interface/SPDamageInterface.h"


// Sets default values for this component's properties
USPExplosionComponent::USPExplosionComponent()
{
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(
		TEXT("/Script/Engine.ParticleSystem'/Game/Box/MagicStaff/Demo/Particles/P_Explosion.P_Explosion'"));
	if (EffectRef.Object)
	{
		Effect = EffectRef.Object;
	}
}


// Called when the game starts
void USPExplosionComponent::BeginPlay()
{
	Super::BeginPlay();
	//this->SetAutoActivate(false);
}

void USPExplosionComponent::Explode()
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
																	EDrawDebugTrace::ForDuration, OutHits, true,
																	GreenColor, RedColor, DrawTime);

	if (Success)
	{
		MultiRPCExplosion(OutHits);
	}
}

void USPExplosionComponent::MultiRPCExplosion_Implementation(const TArray<FHitResult>& OutHits)
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
					DamageInterface->OnExplosionHit();
				}
			}
		}
}



// void USPExplosionComponent::MultiRPCExplosion_Implementation(const TArray<AActor*> ActorArray)
// {
// 	// if (ActorArray.Num() > 0)
// 	// {
// 	// 	for (AActor*& HitActor : ActorArray)
// 	// 	{
// 	// 		ISPDamageInterface* DamageInterface = Cast<ISPDamageInterface>(HitActor);
// 	// 		if (DamageInterface)
// 	// 		{
// 	// 			SP_SUBLOG(LogSPNetwork,Log,TEXT("OnExplosionHit !! "));
// 	// 			DamageInterface->OnExplosionHit();
// 	// 		}
// 	// 	}
// 	// }
// }

//



