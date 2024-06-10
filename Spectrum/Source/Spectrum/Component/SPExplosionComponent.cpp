// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SPExplosionComponent.h"

#include "SpectrumLog.h"
#include "SPGlobalEnum.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/SPDamageInterface.h"

// Sets default values for this component's properties
USPExplosionComponent::USPExplosionComponent()
{
	
	// static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(
	// 	TEXT("/Script/Engine.ParticleSystem'/Game/Box/MagicStaff/Demo/Particles/P_Explosion.P_Explosion'"));
	// if (EffectRef.Object)
	// {
	// 	Effect = EffectRef.Object;
	// }

// 	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitRef(
// TEXT("/Script/Engine.ParticleSystem'/Game/MagicProjectilesVol2/Particles/Hits/CP_BlackPotion.CP_BlackPotion'"));
//
// 	if (HitRef.Succeeded())
// 	{
// 		EmitterHit = HitRef.Object;
// 	}

	//WaterSound = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Spectrum/Sound/Water2.Water2'"));
	//CrushSound = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Spectrum/Sound/Crush.Crush'"));
}


// Called when the game starts
void USPExplosionComponent::BeginPlay()
{
	Super::BeginPlay();
	//this->SetAutoActivate(false);
}

void USPExplosionComponent::Explode(ColorType& MyColor)
{
	Super::Explode(MyColor);
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
		MultiRPCExplosion(MyArray,MyColor);
	}
}
void USPExplosionComponent::MultiRPCExplosion_Implementation(const TArray<AActor*>& MyArray, const ColorType& MyColor)
{
	if (MyArray.Num() > 0)
	{
		for (AActor* HitActor : MyArray)
		{
			if(MyColor == ColorType::Black)
			{
				ISPDamageInterface* DamageInterface = Cast<ISPDamageInterface>(HitActor);
				if (DamageInterface)
				{
					DamageInterface->OnExplosionHit();
				}
			}
			else if(MyColor == ColorType::Green)
			{
				ISPDamageInterface* DamageInterface = Cast<ISPDamageInterface>(HitActor);
				if (DamageInterface)
				{
					DamageInterface->OnChangeColorGreen();
				}
			}
			else if(MyColor == ColorType::Orange)
			{
				ISPDamageInterface* DamageInterface = Cast<ISPDamageInterface>(HitActor);
				if (DamageInterface)
				{
					DamageInterface->OnChangeColorOrange();
				}
			}
			else if(MyColor == ColorType::Purple)
			{
				ISPDamageInterface* DamageInterface = Cast<ISPDamageInterface>(HitActor);
				if (DamageInterface)
				{
					DamageInterface->OnChangeColorPurple();
				}
			}
		}
	}
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterHit, GetOwner()->GetActorLocation(), FRotator::ZeroRotator,
												 FVector(1.0f), true, EPSCPoolMethod::None, true);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), WaterSound, GetOwner()->GetActorLocation());

	if(CrushSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), CrushSound, GetOwner()->GetActorLocation());
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



