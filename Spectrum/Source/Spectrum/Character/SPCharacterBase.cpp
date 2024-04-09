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

	{// 처음 위치를 설정해줌
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

	if (IsMyPlayer() == false)		// 내 플레이어가 아닌 경우에만 DestInfo를 이용하여 이동
	{								// 야금야금 이동하도록 보정

		//if (bIsAiming)
		//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("aim!!!!")));

		const Protocol::MoveState State = DestInfo->state();

		if (State == Protocol::MOVE_STATE_RUN && !bIsJumping)
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
		//	/* 나중에 만약 중간에 패킷을 못받다가 갑자기 받으면 가야하는 위치가 너무 멀어질수도 있으니까
		//	* 이거 어떻게 처리할지 생각해봐야함
		//	* 순간이동이나 그런거로 처리할지, 아니면 서버에서 패킷을 받지 못했다고 알려줄지
		//	* 보정해서 가게 할지
		//	* 아니면 그냥 무시할지
		//	*/
		//}

		//// 중력총 클라이언트 코드 
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

	// 나중에 상태에 따른 애니메이션 변경 추가할수도..?
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

	// Dest에 최종 상태 복사
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

	// 상태만 바로 적용!
	SetMoveState(Info.state());
}

void ASPCharacterBase::AimPotion(const FInputActionValue& Value)
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

void ASPCharacterBase::ThrowPotion(const FInputActionValue& Value)
{
	if (bIsThrowReady)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_JumpToSection(FName("End"), ThrowMontage);
		bIsThrowReady = false;
		if (Potion)
		{
			GetController()->GetControlRotation();
			FVector ForwardVector = UKismetMathLibrary::GetForwardVector(GetController()->GetControlRotation());
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

void ASPCharacterBase::BlackPotionSpawn(const FInputActionValue& Value)
{
	if (IsMyPlayer() == true)
	{
		Protocol::C_O_POTION PoPkt;
		{
			Protocol::PlayerPotionInfo* Info = PoPkt.mutable_info();
			Info->set_object_id(PlayerInfo->object_id());
			Info->set_is_blackspawn(true);
			Info->set_is_throwready(false);
		}

		SEND_PACKET(PoPkt);
	}

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


