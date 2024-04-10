// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SPOrangeExplosionComponent.h"

#include "Interface/SPDamageInterface.h"
#include "Kismet/KismetSystemLibrary.h"

USPOrangeExplosionComponent::USPOrangeExplosionComponent()
{
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
																	EDrawDebugTrace::ForDuration, OutHits, true,
																	GreenColor, RedColor, DrawTime);
	if (Success)
	{
		//for each loop ±¸Çö
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
					DamageInterface->OnChangeColorOrange();
				}
			}
		}
	}
}
