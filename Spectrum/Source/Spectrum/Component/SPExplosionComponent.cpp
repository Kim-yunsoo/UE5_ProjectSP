// Fill out your copyright notice in the Description page of Project Settings.


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
//D:\UE_5.3\Engine\Source\Runtime\Engine\Classes\GameFramework\GameStateBase.h
// Sets default values for this component's properties
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
	
	
	FVector SphereTracePoint = GetOwner()->GetRootComponent()->GetComponentLocation();
	float Radius = 125.f;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
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

	ASPCharacterPlayer* MyOwner = Cast<ASPCharacterPlayer>(GetOwner()->Owner);
	AGameStateBase* State  = GetWorld()->GetGameState();
	ISPScoreInterface* ScoreFuntion = Cast<ISPScoreInterface>(State);

	
	for(FHitResult& Hits : OutHits)
	{
		MyArray.AddUnique(Hits.GetActor());

		ASPCharacterNonPlayer* AIPlayer = Cast<ASPCharacterNonPlayer>(Hits.GetActor());
		if(AIPlayer)
		{
			//색에 따른 추가 구현
			if(MyColor == ColorType::Black ||MyOwner->SchoolAffiliation == MyColor )
			{
				AIPlayer->TakeDamage(BaseDamage * 2, true);
				if (ScoreFuntion)
				{
					ScoreFuntion->AddScore(MyOwner->SchoolAffiliation , EScoreType::AttackSpecial); //State에서 점수를 올려준다. 
				}
				
			}
			else
			{
				AIPlayer->TakeDamage(BaseDamage , true);
				if (ScoreFuntion)
				{
					ScoreFuntion->AddScore(MyOwner->SchoolAffiliation , EScoreType::AttackDefault); //State에서 점수를 올려준다. 
				}
			}
		}
		
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



