// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SPSlowSkill.h"

#include "SpectrumLog.h"
#include "Character/SPCharacterPlayer.h"
#include "SkillActor/SPSlowSkillActor.h"

USPSlowSkill::USPSlowSkill()
{
	PrimaryComponentTick.TickGroup = TG_PostUpdateWork;
	UActorComponent::SetComponentTickEnabled(true);
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled	= true;
	bAutoActivate = true;
	CoolDown = 10;
	// bIsActiveSlowSkill=true;
	static ConstructorHelpers::FObjectFinder<USoundWave> SoundRef(TEXT("/Script/Engine.SoundWave'/Game/Spectrum/Sound/SlowSkill.SlowSkill'"));
	 if(SoundRef.Object)
	 {
	 	SkillSound=SoundRef.Object;
	 }

	//SkillSound= LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Spectrum/Sound/SlowSkill.SlowSkill'"));;
	
}

void USPSlowSkill::BeginPlay()
{
	Super::BeginPlay();
}

void USPSlowSkill::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent( DeltaTime,  TickType,ThisTickFunction);

	if (!Owner)
	{
		return;
	}
	
	if (Owner-> bIsActiveSlowSkill) 
	{
		return;
	}
	FGameTime CurrentGameTime = GetWorld()->GetTime();
	// if(bIsOnce)
	// {
	// 	ActivetedTimeStamp =  CurrentGameTime.GetWorldTimeSeconds();
	// }

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
		Owner->bIsActiveSlowSkill = true;
	}
}

void USPSlowSkill::SkillAction()
{
	Super::SkillAction();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
	FTransform SpawnTransform(Owner->SkillLocation->GetComponentRotation(), Owner->SkillLocation->GetComponentLocation());
	ASPSlowSkillActor* MyActor =GetWorld()->SpawnActorDeferred<ASPSlowSkillActor>(ASPSlowSkillActor::StaticClass(),SpawnTransform);
	MyActor->SetOwner(Owner);
	
	MyActor->FinishSpawning(SpawnTransform);
}

void USPSlowSkill::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void USPSlowSkill::ClientSkillRPC_Implementation(float Time)
{
	SlowCoolDown(Time);
}

