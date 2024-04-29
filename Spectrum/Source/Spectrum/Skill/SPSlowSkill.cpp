// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SPSlowSkill.h"

#include "SpectrumLog.h"
#include "Character/SPCharacterPlayer.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"
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
	
	if (Owner-> bIsActiveSlowSkill) //���̶��? 
	{
		return;
	}

	FGameTime CurrentGameTime = GetWorld()->GetTime();
	if (CurrentGameTime.GetWorldTimeSeconds() < ActivetedTimeStamp + CoolDown)
	{
		float ElapsedTime =( CurrentGameTime.GetWorldTimeSeconds() - ActivetedTimeStamp)/CoolDown;
		float CDTime= FMath::Clamp(1.0f-ElapsedTime, 0.0f, 1.0f);
		// CDTime = FMath::Clamp(CDTime, 0.0f, 1.0f);
		ClientSkillRPC(CDTime);

		// UE_LOG(LogTemp, Log, TEXT("%f"),ElapsedTime); //��Ÿ�� ���� ��
		SP_SUBLOG(LogSPNetwork,Log,TEXT("%f"),ElapsedTime);
		// Owner->bIsActiveSlowSkill = true;
	}
	else
	{
		float ElapsedTime =( CurrentGameTime.GetWorldTimeSeconds() - ActivetedTimeStamp)/CoolDown;

		float CDTime=  FMath::Clamp(1.0f-ElapsedTime, 0.0f, 1.0f)/CoolDown;
		// CDTime = FMath::Clamp(1.0f-ElapsedTime, 0.0f, 1.0f);
		ClientSkillRPC(CDTime);
		UE_LOG(LogTemp, Log, TEXT("TRUE!!"));
		UE_LOG(LogTemp, Log, TEXT("%f"),ElapsedTime);
		// ActivetedTimeStamp=0.0f;
		Owner->bIsActiveSlowSkill = true;
	}
}

void USPSlowSkill::SkillAction(ASPCharacterPlayer* MyOwner)
{
	// OnSkillCool.Broadcast();
	// UE_LOG(LogTemp, Log, TEXT("SkillAction"));
	

	// GameTime = GetWorld()->GetTime();
	// ActivetedTimeStamp = GameTime.GetWorldTimeSeconds();

	Super::SkillAction(MyOwner);
	FVector TracePointStart = Owner->GetActorLocation();
	//this->GetOuter()
	FVector TracePointEnd = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 4000;
	float Radius = 100.f;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Owner);
	TArray<FHitResult> OutHits;
	FLinearColor GreenColor(0.0f, 1.0f, 0.0f);
	FLinearColor RedColor(1.0f, 0.0f, 0.0f);
	float DrawTime = 3.0f;
	bool Success = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), TracePointStart, TracePointEnd,
	                                                                Radius, ObjectTypes, false, ActorsToIgnore,
	                                                                EDrawDebugTrace::ForDuration, OutHits, true,
	                                                                GreenColor, RedColor, DrawTime);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
	if(Success) 
	{
		AActor* HitActor = OutHits[0].GetActor();
		if(Cast<ASPCharacterPlayer>(HitActor))
		{
			// FTransform SpawnLocAndRotation(Owner->SkillLocation->GetComponentRotation(),
			//                                Owner->SkillLocation->GetComponentLocation());
			// ASPSlowSkillActor* MyActor = GetWorld()->SpawnActorDeferred<ASPSlowSkillActor>(
			// 	ASPSlowSkillActor::StaticClass(), SpawnLocAndRotation);
			// MyActor->SetOwner(Owner);
			// MyActor->InitTarget(HitActor);
			// MyActor->FinishSpawning(SpawnLocAndRotation);
			ASPSlowSkillActor* MyActor =GetWorld()->SpawnActor<ASPSlowSkillActor>(ASPSlowSkillActor::StaticClass(),Owner->SkillLocation->GetComponentLocation(),
											   Owner->SkillLocation->GetComponentRotation(), SpawnParams);
			MyActor->SetOwner(Owner);

			
		}
		else
		{
			ASPSlowSkillActor* MyActor =GetWorld()->SpawnActor<ASPSlowSkillActor>(ASPSlowSkillActor::StaticClass(),Owner->SkillLocation->GetComponentLocation(),
												   Owner->SkillLocation->GetComponentRotation(), SpawnParams);
			MyActor->SetOwner(Owner);

		}
	}
	else
	{
		ASPSlowSkillActor* MyActor =GetWorld()->SpawnActor<ASPSlowSkillActor>(ASPSlowSkillActor::StaticClass(),Owner->SkillLocation->GetComponentLocation(),
													   Owner->SkillLocation->GetComponentRotation(), SpawnParams);
		MyActor->SetOwner(Owner);

		UE_LOG(LogTemp,Log,TEXT("SpawnPoint"));
	}
}

void USPSlowSkill::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// DOREPLIFETIME(USPSlowSkill, bIsActiveSlowSkill);
}

void USPSlowSkill::ClientSkillRPC_Implementation(float Time)
{
	SlowCoolDown(Time);
}
