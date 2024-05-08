// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillActor/SPTeleSkillActor.h"
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
	//	FColorData* ColorRow = Table->FindRow<FColorData>(RowName, "");

}

void ASPTeleSkillActor::OnBoxCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HasAuthority()) //���⼭ ��ġ ���� ���ϰ� ������ 
	{
		// float RandomRow = FMath::RandRange()

		if(RangeData)
		{
			float TeleportY = FMath::RandRange(RangeData->MinRow,RangeData->MaxRow);
			float TeleportX = FMath::RandRange(RangeData->MinCol,RangeData->MaxCol);
			float TeleportZ = RangeData->TeleportZ;
			
			MultiRPCTeleSkill(Hit,FVector(TeleportX,TeleportY,TeleportZ));
			this->SetActorHiddenInGame(true);
			this->SetLifeSpan(0.1f);
			
		}
		
	}
}


void ASPTeleSkillActor::MultiRPCTeleSkill_Implementation(const FHitResult& Hit ,const FVector TeleportLocation)
{
	FVector HitLocation = Hit.ImpactPoint;
	if(bIsOnce)
	{
		UParticleSystemComponent* Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterHit, HitLocation, FRotator::ZeroRotator,
												 FVector(1.0f), true, EPSCPoolMethod::None, true);



		bIsOnce=false;
	}
	
	ISPSkillInterface* CheckIceAction = Cast<ISPSkillInterface>(Hit.GetActor());
	if (CheckIceAction)
	{
		CheckIceAction->HitTeleSkillResult(TeleportLocation);
	}
}
