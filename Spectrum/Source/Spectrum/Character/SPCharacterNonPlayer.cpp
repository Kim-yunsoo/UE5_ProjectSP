// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SPCharacterNonPlayer.h"

#include "AI/SPAIController.h"
#include "Component/SPNonCharacterStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Enums/SPMovementSpeed.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	Stat = CreateDefaultSubobject<USPNonCharacterStatComponent>(TEXT("Stat"));
	// Widget Component 
	HpBar = CreateDefaultSubobject<USPWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 210.0f)); //조절 필요
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/Spectrum/UMG/WBP_HpBar.WBP_HpBar_C"));
	if(HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	AIControllerClass = ASPAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ASPCharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &ASPCharacterNonPlayer::SetDead);
}

// Called when the game starts or when spawned
void ASPCharacterNonPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void ASPCharacterNonPlayer::AttackHitCheck()
{
}

void ASPCharacterNonPlayer::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);

	HpBar->SetHiddenInGame(true);

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

float ASPCharacterNonPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Stat->ApplyDamage(DamageAmount);
	
	return DamageAmount;
}

void ASPCharacterNonPlayer::SetupCharacterWidget(USPUserWidget* InUserWidget)
{
	USPHpBarWidget* HpBarWidget = Cast<USPHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->SetMaxHp(Stat->GetMaxHp());
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &USPHpBarWidget::UpdateHpBar);
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
	return  Stat->GetAttackRadius() * 2;
}

float ASPCharacterNonPlayer::GetAITurnSpeed()
{
	return 2.0f;

}

void ASPCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void ASPCharacterNonPlayer::AttackByAI()
{
	//공격 명령 내리는 곳 애니메이션 몽타주 플레이  ProcessComboCommand
	//몽타주가 끝나는 부분 혹은 중단된 부분 NotifyComboActionEnd 호출
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,FTimerDelegate::CreateLambda([&]
	{
		UE_LOG(LogTemp,Log,TEXT("NotifyComboActionEnd"));
		NotifyComboActionEnd();

	}
	),5.0f,false);
}

void ASPCharacterNonPlayer::NotifyComboActionEnd()
{
	//Super::NotifyComboActionEnd();
	OnAttackFinished.ExecuteIfBound(); //델리게이트에 묶인 함수를 호출한다. 
}

float ASPCharacterNonPlayer::SetMovementSpeed(const MovementSpeed MoveSpeed)
{
	if(MoveSpeed == MovementSpeed::Idle)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	}
	else if(MoveSpeed == MovementSpeed::Walking)
	{
		GetCharacterMovement()->MaxWalkSpeed = 100.0f;

	}
	else if(MoveSpeed == MovementSpeed::Jogging)
	{
		GetCharacterMovement()->MaxWalkSpeed =300.0f;

	}
	else if(MoveSpeed == MovementSpeed::Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;

	}
	return GetCharacterMovement()->MaxWalkSpeed;
}


