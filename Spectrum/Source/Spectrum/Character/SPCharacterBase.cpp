// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SPCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SPCharacterControlData.h"
#include "SPCharacterPlayer.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Kismet/KismetMathLibrary.h"


ASPCharacterBase::ASPCharacterBase()
{

	PlayerInfo = new Protocol::PositionInfo();
	DestInfo = new Protocol::PositionInfo();

	//Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	//Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 350.0f;
	GetCharacterMovement()->AirControl = 0.4f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	GetCharacterMovement()->bRunPhysicsWithNoController = true;

	//Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -96.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	//Mesh
	Face = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Face"));
	Face->SetupAttachment(GetMesh());

	Torso = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Torso"));
	Torso->SetupAttachment(GetMesh());

	Legs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
	Legs->SetupAttachment(GetMesh());

	Feet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Feet"));
	Feet->SetupAttachment(GetMesh());

	//staff Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaffMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Spectrum/Assets/Staff/G_Staff/G_Staff.G_Staff'"));
	Staff = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Staff"));
	if (StaffMeshRef.Object)
	{
		Staff->SetStaticMesh(StaffMeshRef.Object);
		Staff->SetupAttachment(GetMesh(), TEXT("Staff_Socket"));
	}

	//Sphere
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshRef(TEXT("/Script/Engine.StaticMesh'/Engine/EditorMeshes/ArcadeEditorSphere.ArcadeEditorSphere'"));
	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	if (SphereMeshRef.Object)
	{
		Sphere->SetStaticMesh(SphereMeshRef.Object);
		Sphere->SetupAttachment(Staff);
		Sphere->SetWorldScale3D(FVector(-0.03125, -0.03125, -0.03125));
		Sphere->SetRelativeLocation(FVector(-2.277422, 0.0, 51.739027));
		Sphere->SetVisibility(false);
		Sphere->SetCollisionProfileName(TEXT("NoCollision"));
	}

	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	//
	//if (CharacterMeshRef.Object)
	//{
	//	GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	//}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Spectrum/Animation/AB_SP_Anim.AB_SP_Anim_c"));

	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<USPCharacterControlData> ShoulderDataRef(TEXT("/Script/Spectrum.SPCharacterControlData'/Game/Spectrum/CharacterControl/SPC_Shoulder.SPC_Shoulder'"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<USPCharacterControlData> QuaterDataRef(TEXT("/Script/Spectrum.SPCharacterControlData'/Game/Spectrum/CharacterControl/SPC_Quater.SPC_Quater'"));
	if (QuaterDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
	}

	PhysicsHandleComponent = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandleComponent"));
	if (PhysicsHandleComponent)
	{
		PhysicsHandleComponent->SetInterpolationSpeed(5.0);
	}



	bIsAiming = false;
}

ASPCharacterBase::~ASPCharacterBase()
{
	delete PlayerInfo;
	delete DestInfo;
	PlayerInfo = nullptr;
	DestInfo = nullptr;

}

void ASPCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	{// ó�� ��ġ�� ��������
		FVector Location = GetActorLocation();
		DestInfo->set_x(Location.X);
		DestInfo->set_y(Location.Y);
		DestInfo->set_z(Location.Z);
		DestInfo->set_yaw(GetControlRotation().Yaw);

		SetMoveState(Protocol::MOVE_STATE_IDLE);
	}
}

void ASPCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	{
		FVector Location = GetActorLocation();
		PlayerInfo->set_x(Location.X);
		PlayerInfo->set_y(Location.Y);
		PlayerInfo->set_z(Location.Z);
		PlayerInfo->set_yaw(GetControlRotation().Yaw);
	}

	if (IsMyPlayer() == false)		// �� �÷��̾ �ƴ� ��쿡�� DestInfo�� �̿��Ͽ� �̵�
	{								// �߱ݾ߱� �̵��ϵ��� ����

		//FVector Location = GetActorLocation();
		//FVector DestLocation = FVector(DestInfo->x(), DestInfo->y(), DestInfo->z());

		//FVector MoveDir = (DestLocation - Location);
		//////const float DistToDest = MoveDir.Length();
		////MoveDir.Length();
		////MoveDir.Normalize();

		////FRotator Rotator = MoveDir.Rotation();
		////float DestLook = Rotator.Yaw;
		//////float LastLook;

		//////float MoveDist = (MoveDir * 600.f * DeltaSeconds).Length();
		//////MoveDist = FMath::Min(MoveDist, DistToDest);	//�����ؼ� ���� �ʰ� ����
		//////FVector NextLocation = Location + MoveDir* MoveDist;

		//SetActorLocation(DestLocation);


		if (bIsAiming)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("aim!!!!")));

		const Protocol::MoveState State = PlayerInfo->state();

		if (State == Protocol::MOVE_STATE_RUN)
		{
			//SetActorRotation(FRotator(0, DestLook, 0));
			SetActorRotation(FRotator(0, DestInfo->yaw() - 90.0f, 0));
			AddMovementInput(GetActorForwardVector());

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("RUN")));
			//LastLook = DestLook;
		}
		else if (State == Protocol::MOVE_STATE_IDLE)
		{
			//SetActorRotation(FRotator(0, LastLook, 0));
			//AddMovementInput(GetActorForwardVector());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("IDLE")));

		}

		//else
		//{
		//	/* ���߿� ���� �߰��� ��Ŷ�� ���޴ٰ� ���ڱ� ������ �����ϴ� ��ġ�� �ʹ� �־������� �����ϱ�
		//	* �̰� ��� ó������ �����غ�����
		//	* �����̵��̳� �׷��ŷ� ó������, �ƴϸ� �������� ��Ŷ�� ���� ���ߴٰ� �˷�����
		//	* �����ؼ� ���� ����
		//	* �ƴϸ� �׳� ��������
		//	*/
		//}

		//// �߷��� Ŭ���̾�Ʈ �ڵ� 
		//if (bIsHolding)
		//{
		//	PhysicsHandleComponent->SetTargetLocation(GravityArrow->K2_GetComponentLocation());
		//}
	}
}

bool ASPCharacterBase::IsMyPlayer()
{
	return Cast<ASPCharacterPlayer>(this) != nullptr;

}

void ASPCharacterBase::SetMoveState(Protocol::MoveState State)
{
	if (PlayerInfo->state() == State)
		return;

	PlayerInfo->set_state(State);

	// ���߿� ���¿� ���� �ִϸ��̼� ���� �߰��Ҽ���..?
}

void ASPCharacterBase::SetPostionInfo(const Protocol::PositionInfo& Info)
{
	if (PlayerInfo->object_id() != 0)
	{
		assert(PlayerInfo->object_id() == Info.object_id());
	}

	PlayerInfo->CopyFrom(Info);
	bIsAiming = Info.is_aiming();
	bIsJumping = Info.is_jumping();
	bIsHolding = Info.is_holding();

	if (IsMyPlayer() == false && Info.is_jumping() == true) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Jump signal 2")));
		SetJumping();
	}
	else if (IsMyPlayer() == false && Info.is_jumping() == false) {
		ResetJumping();
	}

	FVector Location(Info.x(), Info.y(), Info.z());
	SetActorLocation(Location);
}

void ASPCharacterBase::SetDestInfo(const Protocol::PositionInfo& Info)
{
	if (PlayerInfo->object_id() != 0)
	{
		assert(PlayerInfo->object_id() == Info.object_id());
	}

	// Dest�� ���� ���� ����
	DestInfo->CopyFrom(Info);
	bIsAiming = Info.is_aiming();
	//bIsJumping = Info.is_jumping();
	bIsHolding = Info.is_holding();

	if (IsMyPlayer() == false && Info.is_jumping() == true) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Jump signal 2")));
		SetJumping();
	}
	else if (IsMyPlayer() == false && Info.is_jumping() == false) {
		ResetJumping();
	}

	// ���¸� �ٷ� ����!
	SetMoveState(Info.state());
}

void ASPCharacterBase::SetCharacterControlData(const USPCharacterControlData* CharacterControlData)
{
	// Pawn
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

void ASPCharacterBase::SetJumping()
{
	bIsJumping = true;
}

void ASPCharacterBase::ResetJumping()
{
	bIsJumping = false;
}


