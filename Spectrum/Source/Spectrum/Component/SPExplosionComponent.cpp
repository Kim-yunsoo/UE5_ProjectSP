// Fill out your copyright notice in the Description page of Project Settings.



#define COLOR_CASE(ColorName)                \
case ColorType::ColorName :					 \
DamageInterface->OnChangeColor##ColorName(); \
break;										 \

#include "Component/SPExplosionComponent.h"

#include "SpectrumLog.h"
#include "SPGlobalEnum.h"
#include "Character/SPCharacterNonPlayer.h"
#include "Character/SPCharacterPlayer.h"
#include "Enums/SPScoreType.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/SPDamageInterface.h"
#include "Interface/SPScoreInterface.h"
#include "GameFramework\GameStateBase.h"

USPExplosionComponent::USPExplosionComponent()
{
}


// Called when the game starts
void USPExplosionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USPExplosionComponent::Explode(ColorType& MyColor)
{
	Super::Explode(MyColor);

	if(!GetOwner()->HasAuthority())
	{
		return;
	}
	
	FVector SphereTracePoint = GetOwner()->GetRootComponent()->GetComponentLocation();
	float Radius = 125.f;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	//ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> OutHits;
	FLinearColor GreenColor(0.0f, 1.0f, 0.0f);
	FLinearColor RedColor(1.0f, 0.0f, 0.0f);
	float DrawTime = 5.0f;
	bool Success = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), SphereTracePoint, SphereTracePoint,
	                                                                Radius, ObjectTypes, false, ActorsToIgnore,
	                                                                EDrawDebugTrace::None, OutHits, true,
	                                                                GreenColor, RedColor, DrawTime);

	TArray<AActor*> MyArray;

	ASPCharacterPlayer* MyOwner = Cast<ASPCharacterPlayer>(GetOwner()->Owner);
	AGameStateBase* State = GetWorld()->GetGameState();
	ISPScoreInterface* ScoreFuntion = Cast<ISPScoreInterface>(State);


	for (FHitResult& Hits : OutHits)
	{
		MyArray.AddUnique(Hits.GetActor());

		ASPCharacterNonPlayer* AIPlayer = Cast<ASPCharacterNonPlayer>(Hits.GetActor());
		if (AIPlayer)
		{
			//색에 따른 추가 구현
			if (MyColor == ColorType::Black || MyOwner->SchoolAffiliation == MyColor)
			{
				AIPlayer->TakeDamage(BaseDamage * 2, true);
				if (ScoreFuntion)
				{
					ScoreFuntion->AddScore(MyOwner->SchoolAffiliation, EScoreType::AttackSpecial); //State에서 점수를 올려준다. 
				}
			}
			else
			{
				AIPlayer->TakeDamage(BaseDamage, true);
				if (ScoreFuntion)
				{
					ScoreFuntion->AddScore(MyOwner->SchoolAffiliation, EScoreType::AttackDefault); //State에서 점수를 올려준다. 
				}
			}
		}
	}

	if (Success)
	{
		MultiRPCExplosion(MyArray, MyColor);
	}
}





void USPExplosionComponent::MultiRPCExplosion_Implementation(const TArray<AActor*>& MyArray, const ColorType& MyColor)
{
	if (MyArray.Num() > 0)
	{
		for (AActor* HitActor : MyArray)
		{
			ISPDamageInterface* DamageInterface = Cast<ISPDamageInterface>(HitActor);
			if (DamageInterface == nullptr)
			{
				continue;
			}
			switch (MyColor)
			{
				// ColorType값에 따라 영향 , DamageInterface의 함수명에 영향이 있습니다.
				COLOR_CASE(Black);
				COLOR_CASE(Orange);
				COLOR_CASE(Purple);
				COLOR_CASE(Green);
				default:
					break;
			}
		}
	}
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterHit, GetOwner()->GetActorLocation(),
											 FRotator::ZeroRotator,
											 FVector(1.0f), true, EPSCPoolMethod::None, true);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), WaterSound, GetOwner()->GetActorLocation());

	if (CrushSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), CrushSound, GetOwner()->GetActorLocation());
	}
}

