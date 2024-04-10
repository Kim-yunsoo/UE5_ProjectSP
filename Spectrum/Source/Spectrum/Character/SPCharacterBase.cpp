// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SPCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SPCharacterControlData.h"
#include "SPCharacterPlayer.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Spectrum.h"


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
	bIsBlackFour= false;
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

	{// 처占쏙옙 占쏙옙치占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙
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

	if (IsMyPlayer() == false)		// 占쏙옙 占시뤄옙占싱어가 占싣댐옙 占쏙옙荑∽옙占� DestInfo占쏙옙 占싱울옙占싹울옙 占싱듸옙
	{								// 占쌩금야깍옙 占싱듸옙占싹듸옙占쏙옙 占쏙옙占쏙옙

		//if (bIsAiming)
		//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("aim!!!!")));

		const Protocol::MoveState State = DestInfo->state();

		////FRotator Rotator = MoveDir.Rotation();
		////float DestLook = Rotator.Yaw;
		//////float LastLook;

		//////float MoveDist = (MoveDir * 600.f * DeltaSeconds).Length();
		//////MoveDist = FMath::Min(MoveDist, DistToDest);	//占쏙옙占쏙옙占쌔쇽옙 占쏙옙占쏙옙 占십곤옙 占쏙옙占쏙옙
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

			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("RUN")));
			//LastLook = DestLook;
		}
		else if (State == Protocol::MOVE_STATE_IDLE)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("IDLE")));

		}

		//else
		//{
		//	/* 占쏙옙占쌩울옙 占쏙옙占쏙옙 占쌩곤옙占쏙옙 占쏙옙킷占쏙옙 占쏙옙占쌨다곤옙 占쏙옙占쌘깍옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占싹댐옙 占쏙옙치占쏙옙 占십뱄옙 占쌍억옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占싹깍옙
		//	* 占싱곤옙 占쏘떻占쏙옙 처占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쌔븝옙占쏙옙占쏙옙
		//	* 占쏙옙占쏙옙占싱듸옙占싱놂옙 占쌓뤄옙占신뤄옙 처占쏙옙占쏙옙占쏙옙, 占싣니몌옙 占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙킷占쏙옙 占쏙옙占쏙옙 占쏙옙占쌩다곤옙 占싯뤄옙占쏙옙占쏙옙
		//	* 占쏙옙占쏙옙占쌔쇽옙 占쏙옙占쏙옙 占쏙옙占쏙옙
		//	* 占싣니몌옙 占쌓놂옙 占쏙옙占쏙옙占쏙옙占쏙옙
		//	*/
		//}

		//// 占쌩뤄옙占쏙옙 클占쏙옙占싱억옙트 占쌘듸옙 
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

	// 占쏙옙占쌩울옙 占쏙옙占승울옙 占쏙옙占쏙옙 占쌍니몌옙占싱쇽옙 占쏙옙占쏙옙 占쌩곤옙占쌀쇽옙占쏙옙..?
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

	// Dest占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙
	DestInfo->CopyFrom(Info);
	bIsAiming = Info.is_aiming();
	//bIsJumping = Info.is_jumping();
	bIsHolding = Info.is_holding();
	SetIsThrowReady(Info.is_throwpotion());
	SetIsSpawn(Info.is_spawnpotion());

	if (IsMyPlayer() == false && Info.is_jumping() == true) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Jump signal 2")));
		SetJumping();
	}
	else if (IsMyPlayer() == false && Info.is_jumping() == false) {
		ResetJumping();
	}

	// 占쏙옙占승몌옙 占쌕뤄옙 占쏙옙占쏙옙!
	SetMoveState(Info.state());
}



void ASPCharacterBase::SetBlackFour()
{
	if (false == bIsSpawn)
	{
		FVector ItemLocation = GetMesh()->GetSocketLocation("Item_Socket");
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
		Potion = GetWorld()->SpawnActor<ASPBlackPotion>(ASPBlackPotion::StaticClass(), GetMesh()->GetSocketLocation("Item_Socket"), FRotator{ 0.0f, 0.0f, 0.0f }, SpawnParams);
		bIsSpawn = true;
		if (Potion)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
			Potion->AttachToComponent(GetMesh(), AttachmentRules, FName{ "Item_Socket" });
		}
	}
	else
	{
		if (Potion)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
			Potion->AttachToComponent(GetMesh(), AttachmentRules, FName{ "Item_Socket" });
		}
	}
}

void ASPCharacterBase::SetAimPotion()
{
	if (bIsSpawn)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(ThrowMontage, 1.0f);
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		bIsTurnReady = true;
	}
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

void ASPCharacterBase::SetThrowPotion()
{

	if (bIsThrowReady)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_JumpToSection(FName("End"), ThrowMontage);
		bIsThrowReady = false;
		if (Potion)
		{

			FRotator ThrowRotation = FRotator(ThrowPitch, GetActorRotation().Yaw, 0.0f);
			FVector ForwardVector = UKismetMathLibrary::GetForwardVector(ThrowRotation);

			float Mul = 1500.0f;
			Potion->Throw((ForwardVector + FVector{ 0.0f,0.0f,0.4f }) * Mul);
		}

		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		bIsTurnReady = false;
		bIsSpawn = false;
		Potion = nullptr;
	}
	else
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Stop(0.0f);
	}
}

void ASPCharacterBase::SetJumping()
{
	bIsJumping = true;
}

void ASPCharacterBase::ResetJumping()
{
	bIsJumping = false;
}


