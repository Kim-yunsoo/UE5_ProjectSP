// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SPIceSkill.h"
#include "Character/SPCharacterPlayer.h"
#include "SkillActor/SPIceSkillActor.h"

USPIceSkill::USPIceSkill()
{
	PrimaryComponentTick.TickGroup = TG_PostUpdateWork;
	UActorComponent::SetComponentTickEnabled(true);
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled	= true;
	bAutoActivate = true;
	CoolDown = 10;
	
	static ConstructorHelpers::FObjectFinder<USoundWave> SoundRef(TEXT("/Script/Engine.SoundWave'/Game/Spectrum/Sound/IceSkillSound.IceSkillSound'"));
	if(SoundRef.Object)
	{
		SkillSound=SoundRef.Object;
	}
}
void USPIceSkill::BeginPlay()
{
	Super::BeginPlay();
}

void USPIceSkill::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent( DeltaTime,  TickType,ThisTickFunction);

	if (!Owner)
	{
		return;
	}
	if (Owner-> bIsActiveIceSkill) 
	{
		return;
	}

	FGameTime CurrentGameTime = GetWorld()->GetTime();
	if (CurrentGameTime.GetWorldTimeSeconds() < ActivetedTimeStamp + CoolDown)
	{
		float ElapsedTime =( CurrentGameTime.GetWorldTimeSeconds() - ActivetedTimeStamp)/CoolDown;
		float CDTime= FMath::Clamp(1.0f-ElapsedTime, 0.0f, 1.0f);
		ClientSkillRPC(CDTime);
	}
	else
	{
		float ElapsedTime =( CurrentGameTime.GetWorldTimeSeconds() - ActivetedTimeStamp)/CoolDown;

		float CDTime=  FMath::Clamp(1.0f-ElapsedTime, 0.0f, 1.0f)/CoolDown;
		ClientSkillRPC(CDTime);
		Owner->bIsActiveIceSkill = true;
	}
}

void USPIceSkill::SkillAction()
{
	Super::SkillAction();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
	ASPIceSkillActor* MyActor =GetWorld()->SpawnActor<ASPIceSkillActor>(ASPIceSkillActor::StaticClass(),Owner->SkillLocation->GetComponentLocation(),
											   Owner->SkillLocation->GetComponentRotation(), SpawnParams);
	MyActor->SetOwner(Owner);
}

void USPIceSkill::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void USPIceSkill::ClientSkillRPC_Implementation(float Time)
{
	IceCoolDown(Time);
}
