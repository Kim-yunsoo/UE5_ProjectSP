// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SPCharacterNonPlayer.h"

#include "BrainComponent.h"
#include "AI/SPAIController.h"
#include "Component/SPAttackComponent.h"
#include "Component/SPDamageSystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Enums/SPMovementSpeed.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Skill/AISkillActor/SPAIMagicSkill.h"
#include "UI/HpBar/SPHpBarWidget.h"
#include "UI/HpBar/SPWidgetComponent.h"

// Sets default values
ASPCharacterNonPlayer::ASPCharacterNonPlayer()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	//GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_ABCAPSULE); 일단 보류

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	// Stat Component 
	//Stat = CreateDefaultSubobject<USPNonCharacterStatComponent>(TEXT("Stat"));
	// Widget Component 
	HpBar = CreateDefaultSubobject<USPWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 210.0f)); //조절 필요

	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(
		TEXT("/Game/Spectrum/UMG/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	//component
	DamageSystemComponent = CreateDefaultSubobject<USPDamageSystemComponent>(TEXT("DamageSystemComponent"));
	AttackComponent = CreateDefaultSubobject<USPAttackComponent>(TEXT("AttackComponent"));

	AIControllerClass = ASPAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	AttackRadius = 500.0f;
	DefendRadius = 700.0f;
	IsTeleporting=false;
	Attacking = false;
	//
	// MaxHealth=100;
	// Health=100;
	// bIsDead=false;
	//IsDead= false;
}

void ASPCharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	DamageSystemComponent->OnDamageResponse.AddUObject(this, &ASPCharacterNonPlayer::DamageResponse);
	DamageSystemComponent->OnHpZero.AddUObject(this, &ASPCharacterNonPlayer::SetDead);
	DamageSystemComponent->OnDamageResponse.AddUObject(this, &ASPCharacterNonPlayer::HitResponse);
}

void ASPCharacterNonPlayer::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<ASPAIController>(GetController());
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(
		this, &ASPCharacterNonPlayer::HandleMontageAnimNotify);
}

void ASPCharacterNonPlayer::AttackHitCheck()
{
	GetCharacterMovement()->StopMovementImmediately(); //잠깐 멈춘다.
	AIController->SetStateAsFrozen();
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(FireBallMontage, 1.0f);
	//몽타주 재생
	//몽타주 끝나는 부분에 SetStateasAttacking 실행 
}

void ASPCharacterNonPlayer::SetDead()
{
	UE_LOG(LogTemp, Log, TEXT("SetDead"));
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);

	HpBar->SetHiddenInGame(true);

	AIController->BrainComponent->StopLogic(FString{}); // 로직 중단
	AIController->SetStateAsDead();
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		                                       [&]()
		                                       {
			                                       Destroy();
		                                       }
	                                       ), DeadEventDelayTime, false);
}

void ASPCharacterNonPlayer::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	//AnimInstance->Montage_Play(DeadMontage, 1.0f);
}

void ASPCharacterNonPlayer::DamageResponse()
{
	UE_LOG(LogTemp, Log, TEXT("DamageResponse"));
	// add animation
}

// float ASPCharacterNonPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
//                                         AController* EventInstigator, AActor* DamageCauser)
// {
// 	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
//
// 	//Stat->ApplyDamage(DamageAmount);
//
// 	return DamageAmount;
// }

void ASPCharacterNonPlayer::SetupCharacterWidget(USPUserWidget* InUserWidget)
{
	USPHpBarWidget* HpBarWidget = Cast<USPHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->SetMaxHp(DamageSystemComponent->GetMaxHp());
		HpBarWidget->UpdateHpBar(DamageSystemComponent->GetCurrentHp());
		DamageSystemComponent->OnHpChanged.AddUObject(HpBarWidget, &USPHpBarWidget::UpdateHpBar);
	}
}

float ASPCharacterNonPlayer::GetAIPatrolRadius()
{
	return 800.0f;
}

float ASPCharacterNonPlayer::GetAIDetectRange()
{
	return 400.0f;
}

float ASPCharacterNonPlayer::GetAIAttackRange()
{
	//return  Stat->GetAttackRadius() * 2;
	return 0.0f;
}

float ASPCharacterNonPlayer::GetAITurnSpeed()
{
	return 2.0f;
}

void ASPCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void ASPCharacterNonPlayer::SetAITeleportDelegate(const FAICharacterTeleportFinished& InOnTeleportFinished)
{
	OnTeleportFinished = InOnTeleportFinished;
}

void ASPCharacterNonPlayer::Attack(AActor* Target)
{
	Attacking = true;
	MyTarget = Target;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(FireBallMontage, 1.0f);

	FOnMontageEnded CompleteDelegate;
	CompleteDelegate.BindUObject(this, &ASPCharacterNonPlayer::AttackEndDelegate);
	AnimInstance->Montage_SetEndDelegate(CompleteDelegate, FireBallMontage);
}

void ASPCharacterNonPlayer::AttackEndDelegate(UAnimMontage* Montage, bool bInterrupted)
{
	//Super::NotifyComboActionEnd();
	Attacking = false;
	OnAttackFinished.ExecuteIfBound(); //델리게이트에 묶인 함수를 호출한다. 
}

// void ASPCharacterNonPlayer::TeleportEndDelegate(UAnimMontage* Montage, bool bInterrupted)
// {
// 	OnTeleportFinished.ExecuteIfBound();
// }

float ASPCharacterNonPlayer::SetMovementSpeed(const MovementSpeed MoveSpeed)
{
	if (MoveSpeed == MovementSpeed::Idle)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	}
	else if (MoveSpeed == MovementSpeed::Walking)
	{
		GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	}
	else if (MoveSpeed == MovementSpeed::Jogging)
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
	else if (MoveSpeed == MovementSpeed::Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}
	return GetCharacterMovement()->MaxWalkSpeed;
}

float ASPCharacterNonPlayer::GetIdealAttackRange()
{
	return AttackRadius;
}

float ASPCharacterNonPlayer::GetIdealDefendRange()
{
	return DefendRadius; //이 함수는 플레이어를 중심으로 생기는 EQS
}

float ASPCharacterNonPlayer::GetCurrentHealth()
{
	return DamageSystemComponent->Health;
}

float ASPCharacterNonPlayer::GetMaxHealth()
{
	return DamageSystemComponent->MaxHealth;
}

float ASPCharacterNonPlayer::Heal(float Amount)
{
	return DamageSystemComponent->Heal(Amount);
}

bool ASPCharacterNonPlayer::TakeDamage(float Amount, bool ShouldForceInterrupt)
{
	//피해를 입었을 때 호출되는 곳
	// Health = Health - Amount;
	// Health = FMath::Clamp<float>(Health, 0.0f, MaxHealth);
	//
	// if(Health<=0.0f)
	// {
	// 	bIsDead=true;
	// }

	return DamageSystemComponent->TakeDamage(Amount, ShouldForceInterrupt);
}

bool ASPCharacterNonPlayer::IsDead()
{
	return DamageSystemComponent->IsDead;
}

void ASPCharacterNonPlayer::HitResponse()
{
	GetCharacterMovement()->StopMovementImmediately();
	AIController->SetStateAsFrozen(); //맞을땐 Frozen 상태 
	//몽타지 End 부분에 Set State as attack으로 바꿀 것
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(HitMontage, 1.0f);
	//AIController->SetStateAttacking(AIController->AttackTarget,true);

	FOnMontageEnded CompleteDelegate;
	CompleteDelegate.BindUObject(this, &ASPCharacterNonPlayer::HitMontageEnded);
	AnimInstance->Montage_SetEndDelegate(CompleteDelegate, HitMontage);
}

void ASPCharacterNonPlayer::Teleport(FVector Location)
{
	if(!IsTeleporting)
	{
		IsTeleporting=true;
		
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		GetCharacterMovement()->MaxFlySpeed = 5000;
		GetCharacterMovement()->MaxAcceleration = 99999;

		GetMesh()->SetVisibility(false, true);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

		TeleportBodyComponent = UGameplayStatics::SpawnEmitterAttached(TeleportBodyParticle, GetMesh(),
		                                                               FName(TEXT("spine_01")));
		TeleportTrailComponent = UGameplayStatics::SpawnEmitterAttached(TeleportTrailParticle, GetMesh(),
		                                                                FName(TEXT("spine_01")));
		StopAnimMontage();

		AIController->MoveToLocation(Location, 15.0f);
		SetActorLocation(Location);
		TeleportEnd();
	}
	else
	{
		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		LatentInfo.ExecutionFunction = FName("OnTeleportEnd");
		LatentInfo.Linkage = 0;
		LatentInfo.UUID = __LINE__;
		UKismetSystemLibrary::DelayUntilNextTick(this, LatentInfo);
		OnTeleportFinished.ExecuteIfBound();
	}
	// FTimerHandle TimerHandle;
	// GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]
	// {
	// }), 0.7f, false);
}

void ASPCharacterNonPlayer::TeleportEnd()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetCharacterMovement()->MaxAcceleration = 1500.0;
	GetMesh()->SetVisibility(true, true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	OnTeleportFinished.ExecuteIfBound();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		IsTeleporting=false;
		if (TeleportBodyComponent)
		{
			TeleportBodyComponent->DestroyComponent();
		}
		if (TeleportTrailComponent)
		{
			TeleportTrailComponent->DestroyComponent();
		}
	}), 1.0f, false);
}

void ASPCharacterNonPlayer::HandleMontageAnimNotify(FName NotifyName,
                                                    const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == FName("AIFire"))
	{
		FVector Location = GetMesh()->GetSocketLocation(FName(TEXT("RightHand")));

		FTransform Transform = FTransform{FRotator::ZeroRotator, Location, FVector{1.0, 1.0, 1.0,}};

		AttackComponent->MagicSpell(MyTarget, Transform);
	}
}

void ASPCharacterNonPlayer::HitMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	AIController->SetStateAttacking(AIController->AttackTarget, true);
}
