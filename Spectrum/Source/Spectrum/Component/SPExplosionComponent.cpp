// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SPExplosionComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Interface/SPDamageInterface.h"
#include "Particles/ParticleSystem.h"

// Sets default values for this component's properties
USPExplosionComponent::USPExplosionComponent()
{
	
	// static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(
	// 	TEXT("/Script/Engine.ParticleSystem'/Game/Box/MagicStaff/Demo/Particles/P_Explosion.P_Explosion'"));
	// if (EffectRef.Object)
	// {
	// 	Effect = EffectRef.Object;
	// }

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitRef(
TEXT("/Script/Engine.ParticleSystem'/Game/MagicProjectilesVol2/Particles/Hits/CP_BlackPotion.CP_BlackPotion'"));

	if (HitRef.Succeeded())
	{
		EmitterHit = HitRef.Object;
	}

	WaterSound = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Spectrum/Sound/Water2.Water2'"));
	CrushSound = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Spectrum/Sound/Crush.Crush'"));
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
																	EDrawDebugTrace::None, OutHits, true,
																	GreenColor, RedColor, DrawTime);

	TArray<AActor*>MyArray;
	for(FHitResult& Hits : OutHits)
	{
		MyArray.AddUnique(Hits.GetActor());
	}

	if (Success)
	{
		MultiRPCExplosion(MyArray);
	}
}



void USPExplosionComponent::MultiRPCExplosion_Implementation(const TArray<AActor*>& MyArray)
{
	// for (const FHitResult& HitResult : OutHits)
	// {
	// 	AActor* HitActor = HitResult.GetActor();
	// 	//UE_LOG(LogTemp, Warning, TEXT("hit! owne?? %s"), *GetOwner()->GetName());
	// 	
	// 	if (HitActor)
	// 	{
	// 		ActorArray.AddUnique(HitActor);
	// 	}
	// }
	if (MyArray.Num() > 0)
	{
		for (AActor* HitActor : MyArray)
		{
			ISPDamageInterface* DamageInterface = Cast<ISPDamageInterface>(HitActor);
			if (DamageInterface)
			{
				DamageInterface->OnExplosionHit();
			}
		}
	}
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterHit, GetOwner()->GetActorLocation(), FRotator::ZeroRotator,
												 FVector(1.0f), true, EPSCPoolMethod::None, true);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), WaterSound, GetOwner()->GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), CrushSound, GetOwner()->GetActorLocation());

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



