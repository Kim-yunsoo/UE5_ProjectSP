// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillActor/SPTeleSkillActor.h"

#include "Character/SPCharacterPlayer.h"
#include "Components/BoxComponent.h"
#include "Data/SPTeleportData.h"
#include "Interface/SPSkillInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

class ISPSkillInterface;

ASPTeleSkillActor::ASPTeleSkillActor()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> VfxRef(TEXT(
"/Script/Engine.ParticleSystem'/Game/MagicProjectilesVol2/Particles/Projectiles/P_Projectile_ElectricBall01_Yellow.P_Projectile_ElectricBall01_Yellow'"));
	if (VfxRef.Succeeded())
	{
		MainVFX->SetTemplate(VfxRef.Object);
		MainVFX->SetRelativeLocation(FVector(40.0f, 0.0f, 0.0f));
		// MainVFX->SetRelativeRotation(FRotator(0.0f,0.0f,180.f));
		MainVFX->SetRelativeScale3D(FVector(0.5f,0.5f,0.5f));
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitRef(
		TEXT("/Script/Engine.ParticleSystem'/Game/MagicProjectilesVol2/Particles/Hits/CP_TeleHit.CP_TeleHit'"));

	if (HitRef.Succeeded())
	{
		EmitterHit = HitRef.Object;
	}


	static ConstructorHelpers::FObjectFinder<UDataTable> TPRef(
		TEXT("/Script/Engine.DataTable'/Game/Spectrum/Data/DT_TeleportData.DT_TeleportData'"));
	if(TPRef.Object)
	{
		TPData= TPRef.Object;
	}
}

void ASPTeleSkillActor::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentHit.AddDynamic(this, &ASPTeleSkillActor::OnBoxCollisionHit);

	RangeData = TPData->FindRow<FTeleportRangeRowBase>(FName(TEXT("TeleportData")),"");
	if (HasAuthority())
	{
		this->SetReplicates(true);
		this->AActor::SetReplicateMovement(true);
	}

}

void ASPTeleSkillActor::OnBoxCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HasAuthority()) 
	{
		if(bIsOnce && RangeData )
		{
			float TeleportY = FMath::RandRange(RangeData->MinRow, RangeData->MaxRow);
			float TeleportX = FMath::RandRange(RangeData->MinCol, RangeData->MaxCol);
			float TeleportZ = RangeData->TeleportZ;

			FVector TeleportLocation =  {TeleportX,TeleportY,TeleportZ };

			
			
			MultiRPCTeleSkillEffect(Hit.ImpactPoint);
			TArray<AActor*> OverlappedActors;
			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
			TArray<AActor*> ActorsToIgnore;

			bool Result = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 100.f, ObjectTypes,
																	nullptr, ActorsToIgnore, OverlappedActors);
			//DrawDebugSphere(GetWorld(), GetActorLocation(), 100, 32, FColor::Green, false, 5.0f);
			if (Result)
			{
				for (AActor* Actor : OverlappedActors)
				{
					ASPCharacterPlayer* Player = Cast<ASPCharacterPlayer>(Actor);
					if (Player)
					{
						MultiRPCTeleSkill(Player,TeleportLocation);
					}
				}
			}

			this->SetActorHiddenInGame(true);
			this->SetLifeSpan(0.1f);
			bIsOnce=false;
		}
		// if(RangeData)
		// {
		// 	float TeleportY = FMath::RandRange(RangeData->MinRow,RangeData->MaxRow);
		// 	float TeleportX = FMath::RandRange(RangeData->MinCol,RangeData->MaxCol);
		// 	float TeleportZ = RangeData->TeleportZ;
		// 	
		// 	MultiRPCTeleSkill(Hit,FVector(TeleportX,TeleportY,TeleportZ));
		// 	this->SetActorHiddenInGame(true);
		// 	this->SetLifeSpan(0.1f);
		// 	
		// }
		
	}
}

void ASPTeleSkillActor::MultiRPCTeleSkill_Implementation(ASPCharacterPlayer* Player, const FVector& TeleportLocation)
{
	ISPSkillInterface* CheckIceAction = Cast<ISPSkillInterface>(Player);
	if (CheckIceAction)
	{
		CheckIceAction->HitTeleSkillResult(TeleportLocation);
	}
}

void ASPTeleSkillActor::MultiRPCTeleSkillEffect_Implementation(const FVector& InHitLoction)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterHit, InHitLoction, FRotator::ZeroRotator,
												 FVector(1.0f), true, EPSCPoolMethod::None, true);
}


// void ASPTeleSkillActor::MultiRPCTeleSkill_Implementation(const FHitResult& Hit ,const FVector TeleportLocation)
// {
// 	FVector HitLocation = Hit.ImpactPoint;
// 	if(bIsOnce)
// 	{
// 		UParticleSystemComponent* Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterHit, HitLocation, FRotator::ZeroRotator,
// 												 FVector(1.0f), true, EPSCPoolMethod::None, true);
// 		bIsOnce=false;
// 	}
// 	
// 	ISPSkillInterface* CheckIceAction = Cast<ISPSkillInterface>(Hit.GetActor());
// 	if (CheckIceAction)
// 	{
// 		CheckIceAction->HitTeleSkillResult(TeleportLocation);
// 	}
// }
