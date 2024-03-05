// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TestPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TestMyPlayer.h"

//////////////////////////////////////////////////////////////////////////
// ATestPlayer

ATestPlayer::ATestPlayer()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	GetCharacterMovement()->bRunPhysicsWithNoController = true;	// 이거 안하면 서버에서 컨트롤러 없이는 이동 못함



	PlayerInfo = new Protocol::PlayerInfo();
	DestInfo = new Protocol::PlayerInfo();

	
}

ATestPlayer::~ATestPlayer()
{
	delete PlayerInfo;
	delete DestInfo;
	PlayerInfo = nullptr;
	DestInfo = nullptr;
}

void ATestPlayer::BeginPlay()
{
	Super::BeginPlay();

	{// 처음 위치를 설정해줌
		FVector Location = GetActorLocation();
		DestInfo->set_x(Location.X);
		DestInfo->set_y(Location.Y);
		DestInfo->set_z(Location.Z);
		DestInfo->set_yaw(GetControlRotation().Yaw);

		SetMoveState(Protocol::MOVE_STATE_IDLE);
	}
}

void ATestPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	{
		FVector Location = GetActorLocation();
		PlayerInfo->set_x(Location.X);
		PlayerInfo->set_y(Location.Y);
		PlayerInfo->set_z(Location.Z);
		PlayerInfo->set_yaw(GetControlRotation().Yaw);
	}

	if (IsMyPlayer() == false)		// 내 플레이어가 아닌 경우에만 DestInfo를 이용하여 이동
	{								// 야금야금 이동하도록 보정
		//FVector Location = GetActorLocation();
		//FVector DestLocation = FVector(DestInfo->x(), DestInfo->y(), DestInfo->z());

		//FVector MoveDir = (DestLocation - Location);
		//const float DistToDest = MoveDir.Length();
		//MoveDir.Normalize();

		//float MoveDist = (MoveDir * 600.f * DeltaSeconds).Length();
		//MoveDist = FMath::Min(MoveDist, DistToDest);	//오버해서 가지 않게 제한
		//FVector NextLocation = Location + MoveDir* MoveDist;

		//SetActorLocation(NextLocation);

		const Protocol::MoveState State = PlayerInfo->state();

		if (State == Protocol::MOVE_STATE_RUN)
		{
			SetActorRotation(FRotator(0, DestInfo->yaw(), 0));
			AddMovementInput(GetActorForwardVector());
		}
		else
		{
			/* 나중에 만약 중간에 패킷을 못받다가 갑자기 받으면 가야하는 위치가 너무 멀어질수도 있으니까
			* 이거 어떻게 처리할지 생각해봐야함
			* 순간이동이나 그런거로 처리할지, 아니면 서버에서 패킷을 받지 못했다고 알려줄지
			* 보정해서 가게 할지
			* 아니면 그냥 무시할지
			*/
		}
	}
}

bool ATestPlayer::IsMyPlayer()
{
	return Cast<ATestMyPlayer>(this) != nullptr;

}

void ATestPlayer::SetMoveState(Protocol::MoveState State)
{
	if (PlayerInfo->state() == State)
		return;

	PlayerInfo->set_state(State);

	// 나중에 상태에 따른 애니메이션 변경 추가할수도..?

}

void ATestPlayer::SetPlayerInfo(const Protocol::PlayerInfo& Info)
{
	if (PlayerInfo->object_id() != 0)
	{
		assert(PlayerInfo->object_id() == Info.object_id());
	}

	PlayerInfo->CopyFrom(Info);

	FVector Location(Info.x(), Info.y(), Info.z());
	SetActorLocation(Location);
}

void ATestPlayer::SetDestInfo(const Protocol::PlayerInfo& Info)
{

	if (PlayerInfo->object_id() != 0)
	{
		assert(PlayerInfo->object_id() == Info.object_id());
	}

	// Dest에 최종 상태 복사
	DestInfo->CopyFrom(Info);

	// 상태만 바로 적용!
	SetMoveState(Info.state());
}
