// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SPExplosionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Interface/SPDamageInterface.h"


// Sets default values for this component's properties
USPExplosionComponent::USPExplosionComponent()
{
	bHasExecutedOnce = false;
	//파티클 로드
	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/Box/MagicStaff/Demo/Particles/P_Explosion.P_Explosion'"));
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
	if (false == bHasExecutedOnce)
	{
		//Spawn Emitter at Location을 위한 설정
		FVector ActorLocation = GetOwner()->GetActorLocation(); //소유한 액터의 위치 
		FVector Location = ActorLocation;
		FRotator Rotation = FRotator(0.0f,0.0f,0.0f);
		FVector Scale{1.0f,1.0f,1.0f};
		FTransform SpawnTransform{ Rotation,Location,Scale};
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Effect, SpawnTransform, true, EPSCPoolMethod::None, true);

		//Multi Sphere Trace For Object
		//Start & End
		FVector SphereTracePoint = GetOwner()->GetRootComponent()->GetComponentLocation();
		float Radius = 100.f;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		TArray<AActor*> ActorsToIgnore;
		TArray<FHitResult> OutHits;
		FLinearColor GreenColor(0.0f, 1.0f, 0.0f);
		FLinearColor RedColor(1.0f, 0.0f, 0.0f);
		float DrawTime = 5.0f;
		bool Success =UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), SphereTracePoint, SphereTracePoint, Radius, ObjectTypes,false, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHits,true, GreenColor,RedColor, DrawTime);
		if (Success)
		{
			//for each loop 구현
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

						DamageInterface->OnExplosionHit(Damage);
					}
				}
			}
			bHasExecutedOnce= true;
		}
	}
}


