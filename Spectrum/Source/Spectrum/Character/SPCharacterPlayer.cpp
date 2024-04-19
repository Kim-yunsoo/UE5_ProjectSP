// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SPCharacterPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SPCharacterControlData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Spectrum.h"
#include "Object/SPObject.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "Blueprint/UserWidget.h"
#include "Potion/SPBlackPotion.h"
#include "Potion/SPGreenPotion.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SceneComponent.h"
#include "Components/DecalComponent.h"
#include "Components/DynamicMeshComponent.h"
#include "Potion/SPOrangePotion.h"
#include "Potion/SPPurplePotion.h"
#include "SPCharacterMovementComponent.h"
//#include "UI/SPHUDWidget.h"
#include "EngineUtils.h"
#include "SpectrumLog.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"

ASPCharacterPlayer::ASPCharacterPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USPCharacterMovementComponent>(
		ACharacter::CharacterMovementComponentName))
{
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
	Staff = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Staff"));
	Staff->SetupAttachment(GetMesh(), TEXT("Staff_Socket"));
	Staff->SetCollisionProfileName(TEXT("AllCollisionIgnore"));


	//Sphere
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshRef(
		TEXT("/Script/Engine.StaticMesh'/Engine/EditorMeshes/ArcadeEditorSphere.ArcadeEditorSphere'"));
	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	if (SphereMeshRef.Object)
	{
		Sphere->SetStaticMesh(SphereMeshRef.Object);
		Sphere->SetupAttachment(Staff);
		Sphere->SetWorldScale3D(FVector(-0.03125, -0.03125, -0.03125));
		// Sphere->SetRelativeLocation(FVector(-2.277422, 0.0, 51.739027));
		Sphere->SetVisibility(false);
		Sphere->SetCollisionProfileName(TEXT("AllCollisionIgnore"));
	}

	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	//
	//if (CharacterMeshRef.Object)
	//{
	//	GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	//}

	// static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(
	// 	TEXT("/Game/Spectrum/Animation/AB_SP_Anim.AB_SP_Anim_c"));
	//
	// if (AnimInstanceClassRef.Class)
	// {
	// 	GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	// }

	static ConstructorHelpers::FObjectFinder<USPCharacterControlData> ShoulderDataRef(
		TEXT("/Script/Spectrum.SPCharacterControlData'/Game/Spectrum/CharacterControl/SPC_Shoulder.SPC_Shoulder'"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<USPCharacterControlData> QuaterDataRef(
		TEXT("/Script/Spectrum.SPCharacterControlData'/Game/Spectrum/CharacterControl/SPC_Quater.SPC_Quater'"));
	if (QuaterDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
	}

	PhysicsHandleComponent = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandleComponent"));
	if (PhysicsHandleComponent)
	{
		PhysicsHandleComponent->SetInterpolationSpeed(5.0);
	}
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 150.f;
	SpringArm->SocketOffset.Set(0.0, 30, 10);
	SpringArm->bUsePawnControlRotation = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SetRelativeRotation(FRotator(0.0f, -50.f, 0.0f));
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	PotionThrowStartLocation = CreateDefaultSubobject<USceneComponent>(TEXT("PotionThrowStartLocation"));
	PotionThrowStartLocation->SetupAttachment(GetMesh(), FName(TEXT("Item_Socket")));

	Projectile_Path = CreateDefaultSubobject<USplineComponent>(TEXT("Projectile_Path"));
	Projectile_Path->SetupAttachment(RootComponent);


	GravityArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("GravityArrow"));
	if (GravityArrow)
	{
		GravityArrow->SetupAttachment(Staff);
		GravityArrow->SetRelativeRotation(FRotator(0, 0, 0));
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_Jump.IA_SP_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(
		TEXT(
			"/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_ChangeControl.IA_SP_ChangeControl'"));
	if (nullptr != InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_ShoulderMove.IA_SP_ShoulderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_ShoulderLook.IA_SP_ShoulderLook'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_QuaterMove.IA_SP_QuaterMove'"));
	if (nullptr != InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> SpeedUpActionRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_SpeedUp.IA_SP_SpeedUp'"));
	if (nullptr != SpeedUpActionRef.Object)
	{
		SpeedUpAction = SpeedUpActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> AimActionRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_MouseRight.IA_SP_MouseRight'"));
	if (nullptr != AimActionRef.Object)
	{
		MouseRight = AimActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MouseLeftRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_MouseLeft.IA_SP_MouseLeft'"));
	if (nullptr != MouseLeftRef.Object)
	{
		MouseLeft = MouseLeftRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> BlackFourRef(TEXT(
		"/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_BlackPotionSpawn.IA_SP_BlackPotionSpawn'"));
	if (nullptr != BlackFourRef.Object)
	{
		BlackFour = BlackFourRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ThrowCtrlRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_Throw.IA_SP_Throw'"));
	if (nullptr != ThrowCtrlRef.Object)
	{
		ThrowCtrl = ThrowCtrlRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> GreenOneRef(TEXT(
		"/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_GreenPotionSpawn.IA_SP_GreenPotionSpawn'"));
	if (nullptr != GreenOneRef.Object)
	{
		GreenOne = GreenOneRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> OrangeTwoRef(TEXT(
		"/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_OrangePotionSpawn.IA_SP_OrangePotionSpawn'"));
	if (nullptr != OrangeTwoRef.Object)
	{
		OrangeTwo = OrangeTwoRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> PurpleThreeRef(TEXT(
		"/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_PurplePotionSpawn.IA_SP_PurplePotionSpawn'"));
	if (nullptr != PurpleThreeRef.Object)
	{
		PurpleThree = PurpleThreeRef.Object;
	}

	/*static ConstructorHelpers::FObjectFinder<UAnimMontage> ThrowMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Spectrum/Animation/AniMeta/Man/AM_SP_Throw.AM_SP_Throw'"));
	if (ThrowMontageRef.Object)
	{
		ThrowMontage = ThrowMontageRef.Object;
	}*/


	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshforSplineRef(
		TEXT("/Script/Engine.StaticMesh'/Game/Spectrum/SM_MERGED_StaticMeshActor_90.SM_MERGED_StaticMeshActor_90'"));
	if (StaticMeshforSplineRef.Object)
	{
		StaticMeshforSpline = StaticMeshforSplineRef.Object;
	}


	DecalSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DecalSphere"));
	if (DecalSphere)
	{
		DecalSphere->SetupAttachment(RootComponent);
		DecalSphere->SetWorldScale3D(FVector(3.0, 3.0, 3.0));
		DecalSphere->SetCollisionProfileName(TEXT("NoCollision"));
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DecalSphereRef(
		TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (DecalSphereRef.Succeeded())
	{
		DecalSphere->SetStaticMesh(DecalSphereRef.Object);
	}

	MyDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	if (MyDecal)
	{
		MyDecal->SetupAttachment(DecalSphere);
		MyDecal->SetWorldScale3D(FVector(1.0, 0.3, 0.3));
		MyDecal->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	}

	//static ConstructorHelpers::FObjectFinder<UDecalComponent> DecalRef(TEXT("/Script/Engine.Material'/Game/Spectrum/Assets/Decal/M_Circle_Decal.M_Circle_Decal'"));
	//if (DecalSphereRef.Succeeded())
	//{
	//	Decal->SetDecal(DecalRef.Object);
	//}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MeshFinder1(
		TEXT("/Script/Engine.Material'/Game/Spectrum/Assets/Decal/M_Decal_Sphere_Black.M_Decal_Sphere_Black'"));
	if (MeshFinder1.Succeeded())
	{
		DecalSphere->SetMaterial(0, MeshFinder1.Object);
		// 필요에 따라 추가적인 MeshFinder 사용하여 다른 메시 로드 및 추가
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MeshFinder2(
		TEXT("/Script/Engine.Material'/Game/Spectrum/Assets/Decal/M_Decal_Black.M_Decal_Black'"));
	if (MeshFinder2.Succeeded())
	{
		MyDecal->SetMaterial(0, MeshFinder2.Object);
		// 필요에 따라 추가적인 MeshFinder 사용하여 다른 메시 로드 및 추가
	}

	DecalSphere->SetVisibility(false);
	MyDecal->SetVisibility(false);


	CurrentCharacterControlType = ECharacterControlType::Shoulder;
	LastInput = FVector2D::ZeroVector;
	bIsAiming = false;
	bIsHolding = false;
	HitComponent = nullptr;
	bIsSpawn = false;
	bIsThrowReady = false;
	bIsTurnReady = false;
	bIsTurnLeft = false;
	bIsTurnReady = false;
	HitDistance = 1800.f;
}

void ASPCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	SetCharacterControl(CurrentCharacterControlType);
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(
		this, &ASPCharacterPlayer::HandleMontageAnimNotify);
}

void ASPCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsHolding)
	{
		PhysicsHandleComponent->SetTargetLocation(GravityArrow->K2_GetComponentLocation());
	}
}

void ASPCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASPCharacterPlayer::Jumping);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this,
		                                   &ASPCharacterPlayer::MyStopJumping);

		EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this,
		                                   &ASPCharacterPlayer::ChangeCharacterControl);

		EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this,
		                                   &ASPCharacterPlayer::ShoulderMove);

		EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this,
		                                   &ASPCharacterPlayer::ShoulderLook);
		EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::None, this,
		                                   &ASPCharacterPlayer::StopShoulderLook);

		EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this,
		                                   &ASPCharacterPlayer::QuaterMove);
		EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Completed, this,
		                                   &ASPCharacterPlayer::QuaterMove);

		EnhancedInputComponent->BindAction(SpeedUpAction, ETriggerEvent::Triggered, this, &ASPCharacterPlayer::SpeedUp);
		EnhancedInputComponent->BindAction(SpeedUpAction, ETriggerEvent::Completed, this,
		                                   &ASPCharacterPlayer::StopSpeedUp);

		EnhancedInputComponent->BindAction(MouseLeft, ETriggerEvent::Triggered, this, &ASPCharacterPlayer::Graping);
		EnhancedInputComponent->BindAction(MouseLeft, ETriggerEvent::Completed, this, &ASPCharacterPlayer::StopGraping);

		EnhancedInputComponent->BindAction(MouseRight, ETriggerEvent::Triggered, this, &ASPCharacterPlayer::Aiming);
		EnhancedInputComponent->BindAction(MouseRight, ETriggerEvent::Completed, this, &ASPCharacterPlayer::StopAiming);

		EnhancedInputComponent->BindAction(BlackFour, ETriggerEvent::Triggered, this,
		                                   &ASPCharacterPlayer::BlackPotionSpawn);

		EnhancedInputComponent->BindAction(ThrowCtrl, ETriggerEvent::Triggered, this, &ASPCharacterPlayer::AimPotion);
		EnhancedInputComponent->BindAction(ThrowCtrl, ETriggerEvent::Completed, this, &ASPCharacterPlayer::ThrowPotion);

		EnhancedInputComponent->BindAction(GreenOne, ETriggerEvent::Triggered, this,
		                                   &ASPCharacterPlayer::GreenPotionSpawn);
		EnhancedInputComponent->BindAction(OrangeTwo, ETriggerEvent::Triggered, this,
		                                   &ASPCharacterPlayer::OrangePotionSpawn);
		EnhancedInputComponent->BindAction(PurpleThree, ETriggerEvent::Triggered, this,
		                                   &ASPCharacterPlayer::PurplePotionSpawn);
	}
}

void ASPCharacterPlayer::SetCharacterControlData(const USPCharacterControlData* CharacterControlData)
{
	// Pawn
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

void ASPCharacterPlayer::CameraMove()
{
	FLatentActionInfo LatentInfo;
	FVector TargetRelativeLocation{0, 0, 0};
	FRotator TargetRelativeRotation{0, 0, 0};
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(FollowCamera, TargetRelativeLocation, TargetRelativeRotation, true, true, 0.5,
	                                      true, EMoveComponentAction::Type::Move, LatentInfo);
}

void ASPCharacterPlayer::ChangeCharacterControl()
{
	if (CurrentCharacterControlType == ECharacterControlType::Quater)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quater);
	}
}

void ASPCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	if (!IsLocallyControlled())
	{
		SP_LOG(LogSPNetwork, Log, TEXT("%s"), TEXT("IsLocallyControlled"));
		return;
	}

	USPCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

	CurrentCharacterControlType = NewCharacterControlType;
}

void ASPCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.X);
		AddMovementInput(RightDirection, MovementVector.Y);

		{
			DesiredInput = MovementVector;

			DesiredMoveDirection = FVector::ZeroVector;
			DesiredMoveDirection -= ForwardDirection * MovementVector.Y;
			DesiredMoveDirection += RightDirection * MovementVector.X;
			DesiredMoveDirection.Normalize();

			const FVector Location = GetActorLocation();
			FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(Location, Location + DesiredMoveDirection);
			DesiredYaw = Rotator.Yaw;
		}

		/*	ClearTurnInPlace(MovementVector.X);
			ClearTurnInPlace(MovementVector.Y);*/
	}
}

void ASPCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);

		if (GetBaseAimRotation().Yaw > PreControlYawRotation)
		{
			//UE_LOG(LogTemp, Log, TEXT("TEST1"));
			bIsTurnRight = true;
			bIsTurnLeft = false;
			if (!HasAuthority())
				ServerRPCdirection(bIsTurnRight, bIsTurnLeft);
		}
		else
		{
			bIsTurnRight = false;
			bIsTurnLeft = true;
			if (!HasAuthority())
				ServerRPCdirection(bIsTurnRight, bIsTurnLeft);
		}
		PreControlYawRotation = GetBaseAimRotation().Yaw;
	}
}

void ASPCharacterPlayer::StopShoulderLook(const FInputActionValue& Value)
{
	bIsTurnRight = false;
	bIsTurnLeft = false;
	if (!HasAuthority())
		ServerRPCdirection(bIsTurnRight, bIsTurnLeft);
}

void ASPCharacterPlayer::SpeedUp(const FInputActionValue& Value)
{
	if (false == bIsAiming && false == bIsHolding)
	{
		if (!HasAuthority())
		{
			GetCharacterMovement()->MaxWalkSpeed = 900.f;
		}
		ServerRPCSpeedUp();
	}
}

void ASPCharacterPlayer::StopSpeedUp(const FInputActionValue& Value)
{
	if (!HasAuthority())
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
	}
	ServerRPCSpeedUpStop();
}

void ASPCharacterPlayer::Aiming(const FInputActionValue& Value)
{
	if (false == bIsHolding)
	{
		Aiming_CameraMove(); //애니메이션 작동
		bIsAiming = true;
		if (!HasAuthority())
		{
			ServerRPCAiming();
		}
	}
}

void ASPCharacterPlayer::ServerRPCAiming_Implementation()
{
	bIsAiming = true;
}

void ASPCharacterPlayer::StopAiming(const FInputActionValue& Value)
{
	bIsAiming = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	FollowCamera->K2_AttachToComponent(CameraBoom, NAME_None, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld,
	                                   EAttachmentRule::KeepWorld, true);
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld,
	                                          EAttachmentRule::KeepWorld, true);
	FollowCamera->AttachToComponent(CameraBoom, AttachmentRules, NAME_None);
	CameraMove();
	if (!HasAuthority())
	{
		ServerRPCStopAiming();
	}
}

void ASPCharacterPlayer::ServerRPCStopAiming_Implementation()
{
	bIsAiming = false;
}

void ASPCharacterPlayer::Graping(const FInputActionValue& Value)
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	if (false == bIsHolding)
	{
		//FVector SphereLocationStart = Sphere->K2_GetComponentLocation();
		FVector SphereLocationStart = FollowCamera->K2_GetComponentLocation();
		//FVector SphereLocationEnd = SphereLocationStart + (1500 * FollowCamera->GetForwardVector());
		APlayerController* PlayerController = GetController<APlayerController>();
		if (PlayerController != nullptr)
		{
			FRotator ControlRotation = PlayerController->GetControlRotation();
			FVector ReseltFoward = UKismetMathLibrary::GetForwardVector(ControlRotation);

			FVector WorldLocation;
			FVector WorldDirection;
			bool TransSuccess = PlayerController->DeprojectScreenPositionToWorld(
				0.5, 0.5, WorldLocation, WorldDirection);

			FVector SphereLocationEnd = ReseltFoward * 10000000 + SphereLocationStart;

			TArray<TEnumAsByte<EObjectTypeQuery>> EmptyObjectTypes;
			EDrawDebugTrace::Type drawDebugType = EDrawDebugTrace::ForDuration;
			TArray<AActor*> HitActorsToIgnore;
			FLinearColor RedColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
			FLinearColor GreenColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);
			FCollisionQueryParams Params;
			//모든 캐릭터 타입은 무시하도록 하자,
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), HitActorsToIgnore);
			for (AActor* FoundActor : HitActorsToIgnore)
			{
				Params.AddIgnoredActor(FoundActor);
			}
			// Params.AddIgnoredActor(this);
			Params.bTraceComplex = true;
			float DrawTime = 5.0f;

			bool HitSuccess = GetWorld()->LineTraceSingleByChannel(outHitResult, SphereLocationStart, SphereLocationEnd,
			                                                       ECC_GameTraceChannel1, Params);
			// UActorComponent* DynamicMeshComponent = outHitResult.GetActor()->GetComponentByClass(UDynamicMeshComponent::StaticClass());
			// if(HitSuccess && DynamicMeshComponent)
			// {
			// 	Cast<UDynamicMeshComponent>(DynamicMeshComponent)->SetComplexAsSimpleCollisionEnabled(false,true);
			// }
			if (HitSuccess && outHitResult.Component->Mobility == EComponentMobility::Movable)
			{
				outHitResult.Component->SetSimulatePhysics(true);
				HitComponent = outHitResult.GetComponent();


				// AActor* HitActor = outHitResult.GetActor();
				// if (HitActor)
				// {
				// 	UStaticMeshComponent* MeshComponent = HitActor->FindComponentByClass<UStaticMeshComponent>();
				// 	// MeshComponent->SetCollisionEnabled();
				// }

				//여기서 주변 물체의 SetSimulatePhysics(true);
				FVector SphereTracePoint = HitComponent->K2_GetComponentLocation();
				float Radius = 150.f;
				TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
				ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
				TArray<AActor*> ActorsToIgnore;
				ActorsToIgnore.Add(this);
				TArray<FHitResult> OutHits;
				FLinearColor GreenColor1(0.0f, 1.0f, 0.0f);
				FLinearColor RedColor1(1.0f, 0.0f, 0.0f);
				float DrawTime1 = 5.0f;

				bool Success = UKismetSystemLibrary::SphereTraceMultiForObjects(
					GetWorld(), SphereTracePoint, SphereTracePoint, Radius, ObjectTypes, false, ActorsToIgnore,
					EDrawDebugTrace::ForDuration, OutHits, true, GreenColor1, RedColor1, DrawTime1);

				ActorPrimitiveArray.Empty();

				if (Success)
				{
					for (const FHitResult& HitResult : OutHits)
					{
						AActor* Hit = HitResult.GetActor();
						UPrimitiveComponent* PrimitiveHit = Cast<UPrimitiveComponent>(Hit->GetRootComponent());
						if (PrimitiveHit)
						{
							ActorPrimitiveArray.AddUnique(PrimitiveHit);
						}
					}
					if (ActorPrimitiveArray.Num() > 0)
					{
						for (UPrimitiveComponent*& HitPrimitive : ActorPrimitiveArray)
						{
							if (HitPrimitive->Mobility == EComponentMobility::Movable)
							{
								HitPrimitive->SetSimulatePhysics(true);
							}
						}
					}
				}
				if (HitComponent && HitComponent->IsSimulatingPhysics())
				{
					PhysicsHandleComponent->GrabComponentAtLocation(
						HitComponent,
						NAME_None,
						HitComponent->K2_GetComponentLocation()
					);

					bIsHolding = true;

					FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld,
					                                          EAttachmentRule::KeepWorld, true);
					FollowCamera->AttachToComponent(CameraBoom, AttachmentRules, NAME_None);
					CameraMove();
				}
			}

			const FColor LineColor = HitSuccess ? FColor::Green : FColor::Red;

			DrawDebugLine(
				GetWorld(),
				SphereLocationStart,
				SphereLocationEnd,
				LineColor,
				false,
				5.0f,
				0,
				1.0f
			);

			if (HitSuccess)
			{
				DrawDebugPoint(
					GetWorld(),
					outHitResult.ImpactPoint,
					10.0f,
					FColor::Blue,
					false,
					5.0f
				);
			}
		}
	}
	else
	{
		bIsHolding = false;
		if (HitComponent && HitComponent->IsSimulatingPhysics())
		{
			PhysicsHandleComponent->ReleaseComponent();
			HitComponent->AddImpulse(FollowCamera->GetForwardVector() * HitDistance, NAME_None, true);
			HitComponent = nullptr;
		}
	}
}

void ASPCharacterPlayer::StopGraping(const FInputActionValue& Value)
{
	if (bIsHolding && HitComponent->IsSimulatingPhysics())
	{
		bIsHolding = false;
		PhysicsHandleComponent->ReleaseComponent();
		HitComponent->AddImpulse(FollowCamera->GetForwardVector() * HitDistance, NAME_None, true);
		HitComponent = nullptr;
	}
}

void ASPCharacterPlayer::AimPotion(const FInputActionValue& Value)
{
	if (!HasAuthority())
	{
		if (bIsSpawn)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			UAnimInstance* TorsoAnimInstance = Torso->GetAnimInstance();
			AnimInstance->Montage_Play(ThrowMontage, 1.0f);
			// TorsoAnimInstance->Montage_Play(ThrowMontage, 1.0f);
			GetCharacterMovement()->bOrientRotationToMovement = false;
			GetCharacterMovement()->bUseControllerDesiredRotation = true;
			bIsTurnReady = true;
			ServerRPCTurnReady();
		}
	}
}

void ASPCharacterPlayer::ThrowPotion(const FInputActionValue& Value)
{
	if(!HasAuthority())
	{
		if (bIsThrowReady)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			UAnimInstance* TorsoAnimInstance = GetMesh()->GetAnimInstance();
			AnimInstance->Montage_JumpToSection(FName("End"), ThrowMontage);
			// TorsoAnimInstance->Montage_JumpToSection(FName("End"), ThrowMontage);
			bIsThrowReady = false;
			if (Potion)
			{
				GetController()->GetControlRotation();
				FVector ForwardVector = UKismetMathLibrary::GetForwardVector(GetController()->GetControlRotation());
				float Mul = 1500.0f;
				Potion->Throw((ForwardVector + FVector{0.0f, 0.0f, 0.4f}) * Mul);
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
			UAnimInstance* TorsoAnimInstance = GetMesh()->GetAnimInstance();
			AnimInstance->Montage_Stop(0.0f);
			// TorsoAnimInstance->Montage_Stop(0.0f);
		}
	}
	ServerRPCThrowPotion();
}

void ASPCharacterPlayer::Jumping(const FInputActionValue& Value)
{
	if (!bIsAiming)
	{
		bPressedJump = true;
		JumpKeyHoldTime = 0.0f;
	}
}

void ASPCharacterPlayer::MyStopJumping(const FInputActionValue& Value)
{
	bPressedJump = false;
	ResetJumpState();
}

void ASPCharacterPlayer::BlackPotionSpawn(const FInputActionValue& Value)
{
	ServerRPCBlackPotionSpawn();
}

void ASPCharacterPlayer::GreenPotionSpawn(const FInputActionValue& Value)
{
	if (false == bIsSpawn)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
		Potion = GetWorld()->SpawnActor<ASPGreenPotion>(ASPGreenPotion::StaticClass(),
		                                                GetMesh()->GetSocketLocation("Item_Socket"),
		                                                FRotator{0.0f, 0.0f, 0.0f}, SpawnParams);
		//Potion->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		//Potion->SetupAttachment(RootComponent);
		//Potion->RegisterComponent();
		bIsSpawn = true;
		if (Potion)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
			                                          EAttachmentRule::SnapToTarget, true);
			Potion->AttachToComponent(GetMesh(), AttachmentRules, FName{"Item_Socket"});
		}
	}
	else
	{
		if (Potion)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
			                                          EAttachmentRule::SnapToTarget, true);
			Potion->AttachToComponent(GetMesh(), AttachmentRules, FName{"Item_Socket"});
		}
	}
}

void ASPCharacterPlayer::OrangePotionSpawn(const FInputActionValue& Value)
{
	if (false == bIsSpawn)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
		Potion = GetWorld()->SpawnActor<ASPOrangePotion>(ASPOrangePotion::StaticClass(),
		                                                 GetMesh()->GetSocketLocation("Item_Socket"),
		                                                 FRotator{0.0f, 0.0f, 0.0f}, SpawnParams);
		bIsSpawn = true;
		if (Potion)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
			                                          EAttachmentRule::SnapToTarget, true);
			Potion->AttachToComponent(GetMesh(), AttachmentRules, FName{"Item_Socket"});
		}
	}
	else
	{
		if (Potion)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
			                                          EAttachmentRule::SnapToTarget, true);
			Potion->AttachToComponent(GetMesh(), AttachmentRules, FName{"Item_Socket"});
		}
	}
}

void ASPCharacterPlayer::PurplePotionSpawn(const FInputActionValue& Value)
{
	if (false == bIsSpawn)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
		Potion = GetWorld()->SpawnActor<ASPPurplePotion>(ASPPurplePotion::StaticClass(),
		                                                 GetMesh()->GetSocketLocation("Item_Socket"),
		                                                 FRotator{0.0f, 0.0f, 0.0f}, SpawnParams);
		bIsSpawn = true;
		if (Potion)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
			                                          EAttachmentRule::SnapToTarget, true);
			Potion->AttachToComponent(GetMesh(), AttachmentRules, FName{"Item_Socket"});
		}
	}
	else
	{
		if (Potion)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
			                                          EAttachmentRule::SnapToTarget, true);
			Potion->AttachToComponent(GetMesh(), AttachmentRules, FName{"Item_Socket"});
		}
	}
}

void ASPCharacterPlayer::OnRep_Potion()
{
	SP_LOG(LogSPNetwork, Log, TEXT("%s"), TEXT("Potion"));
	if (!Potion)
	{
		SP_LOG(LogSPNetwork, Log, TEXT("%s"), TEXT("ISPotion"));
	}
	if (Potion)
	{
		SP_LOG(LogSPNetwork, Log, TEXT("%s"), TEXT("Potion YSE"));
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
		                                          EAttachmentRule::SnapToTarget, true);
		Potion->AttachToComponent(GetMesh(), AttachmentRules, FName{"Item_Socket"});
	}
}

void ASPCharacterPlayer::OnRep_PotionSpawn()
{
	SP_LOG(LogSPNetwork, Log, TEXT("%s"), TEXT("Potionspawn"));
	
	// if (Potion)
	// {
	//
	// 	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
	// 	                                          EAttachmentRule::SnapToTarget, true);
	// 	Potion->AttachToComponent(GetMesh(), AttachmentRules, FName{"Item_Socket"});
	// }
}

void ASPCharacterPlayer::PlayTurnAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	UAnimInstance* TorsoAnimInstance = Torso->GetAnimInstance();
	AnimInstance->Montage_Play(ThrowMontage, 1.0f);
	// TorsoAnimInstance->Montage_Play(ThrowMontage, 1.0f);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void ASPCharacterPlayer::ServerRPCdirection_Implementation(bool TurnRight, bool Turnleft)
{
	bIsTurnRight = TurnRight;
	bIsTurnLeft = Turnleft;
}

void ASPCharacterPlayer::ClientRPCTurnAnimation_Implementation(ASPCharacterPlayer* CharacterToPlay)
{
	if (CharacterToPlay)
	{
		CharacterToPlay->PlayTurnAnimation();
	}
}

void ASPCharacterPlayer::ServerRPCTurnReady_Implementation()
{
	if (!bIsTurnReady)
	{
		bIsTurnReady = true;
		PlayTurnAnimation();
		
		for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld())) //플레이어 컨트롤러 목록을 서버에서 가지고 오기
		{
			if (PlayerController && GetController() != PlayerController) //시뮬레이트 프록시
			{
				if(!PlayerController->IsLocalController())
				{
					//서버 아니고 공격 명령 내린 플레이어 컨트롤러도 아닌 시뮬레이트 프록시
					//폰을 재생하는 플레이어 컨트롤러
					ASPCharacterPlayer* OtherPlayer = Cast<ASPCharacterPlayer>(PlayerController->GetPawn());
					if (OtherPlayer)
					{
						OtherPlayer->ClientRPCTurnAnimation(this);
					}
				}
			}
		}
	}
}

void ASPCharacterPlayer::ServerRPCThrowPotion_Implementation()
{

}

void ASPCharacterPlayer::HandleMontageAnimNotify(FName NotifyName,
                                                 const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == FName("PlayMontageNotify"))
	{
		bIsThrowReady = true;
		ShowProjectilePath();
	}
}

void ASPCharacterPlayer::ShowProjectilePath()
{
	Projectile_Path->ClearSplinePoints(true);
	for (int i = 0; i < SplineCompArray.Num(); i++)
	{
		SplineCompArray[i]->DestroyComponent();
	}
	SplineCompArray.Empty();
	if (bIsThrowReady)
	{
		FPredictProjectilePathParams PredictParams;
		FPredictProjectilePathResult PredictResult;

		FHitResult OutHit;
		TArray<FVector> OutPathPositions;
		FVector OutLastTraceDestination;

		FVector StartPos = PotionThrowStartLocation->GetComponentLocation();
		//GetController()->GetControlRotation();
		//FVector LaunchVelocity = ; 
		FVector LaunchVelocity = (UKismetMathLibrary::GetForwardVector(GetController()->GetControlRotation())
			+ FVector{0.0f, 0.0f, 0.4f}) * 1500.0f;
		//(ForwardVector + FVector{ 0.0f,0.0f,0.4f })* Mul
		float ProjectileRadius = 0.0f;
		TEnumAsByte<ECollisionChannel> TraceChannel = ECollisionChannel::ECC_Camera;
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);
		EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None;
		float DrawDebugTime = 0.0f;
		float SimFrequency = 15.0f;
		float MaxSimTime = 2.0f;
		float OverrideGravityZ = 0.0;

		UGameplayStatics::Blueprint_PredictProjectilePath_ByTraceChannel(GetWorld(), OutHit, OutPathPositions,
		                                                                 OutLastTraceDestination, StartPos,
		                                                                 LaunchVelocity, true, ProjectileRadius,
		                                                                 TraceChannel, false, ActorsToIgnore,
		                                                                 DrawDebugType, DrawDebugTime, SimFrequency,
		                                                                 MaxSimTime, OverrideGravityZ);

		FHitResult SweepHitResult;
		/*ProjectileCircle->SetWorldLocation(OutHit.Location, false, &SweepHitResult, ETeleportType::None);
		ProjectileCircle->SetVisibility(true, false);*/

		//FVector DecalSize{ 100,200,200 };
		//UGameplayStatics::SpawnDecalAtLocation(GetWorld(), Decal, DecalSize, OutHit.Location, GetControlRotation(), 0.1);
		//UE_LOG(LogTemp, Log, TEXT("TEST"));
		//DecalSphere->SetStaticMesh();
		//DecalSphere->SetStaticMesh(MeshArray[1]);

		//DecalSphere->SetVisibility(true);

		//Decal->SetVisibility(true);
		//DecalSphere->SetWorldLocation(OutHit.Location, false, &SweepHitResult, ETeleportType::None);
		//DecalSphere->SetVisibility(true, false);

		DecalSphere->SetVisibility(true);
		MyDecal->SetVisibility(true);
		DecalSphere->SetWorldLocation(OutHit.Location, false, &SweepHitResult, ETeleportType::TeleportPhysics);

		for (int i = 0; i < OutPathPositions.Num(); i++)
		{
			Projectile_Path->AddSplinePointAtIndex(OutPathPositions[i], i, ESplineCoordinateSpace::Type::Local, true);
		}
		for (int i = 0; i < Projectile_Path->GetNumberOfSplinePoints() - 1; ++i)
		{
			UClass* whyClass = USplineMeshComponent::StaticClass();
			FTransform RelativeTransform = FTransform();

			USplineMeshComponent* NewSplineMeshComp = NewObject<USplineMeshComponent>(
				this, USplineMeshComponent::StaticClass());
			//UActorComponent* ActorComponent = AddComponentByClass(USplineMeshComponent::StaticClass(), true, RelativeTransform, false);
			//USplineMeshComponent* NewSplineMeshComp = Cast<USplineMeshComponent>(ActorComponent);

			if (NewSplineMeshComp == nullptr)
			{
				continue;
			}
			float Radius = 50.0f;
			FColor Color1 = FColor::Red;
			FColor Color2 = FColor::Blue;
			FColor Color3 = FColor::Black;

			NewSplineMeshComp->OnComponentCreated();
			NewSplineMeshComp->SetRelativeTransform(RelativeTransform);
			//NewSplineMeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			//NewSplineMeshComp->SetupAttachment(RootComponent);
			NewSplineMeshComp->SetStaticMesh(StaticMeshforSpline);
			NewSplineMeshComp->SetMobility(EComponentMobility::Movable);
			NewSplineMeshComp->SetCollisionProfileName(TEXT("SplineCollision"));
			//NewSplineMeshComp->SetGenerateOverlapEvents(true);
			/*if (StaticMeshforSpline)
			{
				UE_LOG(LogTemp, Log, TEXT("MeshName: [%s]"), *GetNameSafe(StaticMeshforSpline));
			}*/

			FVector StartPointLocation;
			FVector StartPointTangent;
			FVector EndPointLocation;
			FVector EndPointTangent;


			bool bIsSuccessStart = false;
			bool bIsSuccessEnd = false;
			if (i < Projectile_Path->GetNumberOfSplinePoints())
			{
				StartPointLocation = Projectile_Path->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
				StartPointTangent = Projectile_Path->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
				bIsSuccessStart = true;
			}

			if (i + 1 < Projectile_Path->GetNumberOfSplinePoints())
			{
				EndPointLocation = Projectile_Path->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
				EndPointTangent = Projectile_Path->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
				bIsSuccessEnd = true;
			}
			if (bIsSuccessStart && bIsSuccessEnd)
			{
				NewSplineMeshComp->SetStartAndEnd(StartPointLocation, StartPointTangent, EndPointLocation,
				                                  EndPointTangent, true);
			}
			SplineCompArray.Emplace(NewSplineMeshComp);
			NewSplineMeshComp->RegisterComponent();
		}
		FTimerHandle TimerHandle;
		float DelayTime = 0.01f;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			ShowProjectilePath();
		}, DelayTime, false);
	}
	else
	{
		//ProjectileCircle->SetVisibility(false);
		DecalSphere->SetVisibility(false);
		MyDecal->SetVisibility(false);
	}
}


void ASPCharacterPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASPCharacterPlayer, bIsSpawn);
	DOREPLIFETIME(ASPCharacterPlayer, bIsAiming);
	DOREPLIFETIME(ASPCharacterPlayer, Potion);
	DOREPLIFETIME(ASPCharacterPlayer, bIsTurnLeft);
	DOREPLIFETIME(ASPCharacterPlayer, bIsTurnRight);
}

void ASPCharacterPlayer::ServerRPCBlackPotionSpawn_Implementation()
{
	SP_LOG(LogSPNetwork, Log, TEXT("%s"), TEXT("Spawn"));
	if (false == bIsSpawn)
	{
		FVector ItemLocation = GetMesh()->GetSocketLocation("Item_Socket");
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
		Potion = GetWorld()->SpawnActor<ASPBlackPotion>(ASPBlackPotion::StaticClass(),
		                                                GetMesh()->GetSocketLocation("Item_Socket"),
		                                                FRotator{0.0f, 0.0f, 0.0f}, SpawnParams);
		bIsSpawn = true;
		if (Potion)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
			                                          EAttachmentRule::SnapToTarget, true);
			Potion->AttachToComponent(GetMesh(), AttachmentRules, FName{"Item_Socket"});
		}
	}
	else
	{
		if (Potion)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
			                                          EAttachmentRule::SnapToTarget, true);
			Potion->AttachToComponent(GetMesh(), AttachmentRules, FName{"Item_Socket"});
		}
	}
	//MulticastRPCPotion();
}


void ASPCharacterPlayer::Aiming_CameraMove()
{
	if (false == bIsHolding)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld,
		                                          EAttachmentRule::KeepWorld, true);
		FollowCamera->AttachToComponent(SpringArm, AttachmentRules, NAME_None);
		CameraMove();
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld,
		                                          EAttachmentRule::KeepWorld, true);
		FollowCamera->AttachToComponent(CameraBoom, AttachmentRules, NAME_None);
		CameraMove();
	}
}

void ASPCharacterPlayer::ServerRPCSpeedUpStop_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
}

void ASPCharacterPlayer::ServerRPCSpeedUp_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 900.f;
}

void ASPCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);


	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.0f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
	{
		DesiredInput = MovementVector;

		DesiredMoveDirection = FVector::ZeroVector;
		DesiredMoveDirection += ForwardDirection * MovementVector.Y;
		DesiredMoveDirection += RightDirection * MovementVector.X;
		DesiredMoveDirection.Normalize();

		const FVector Location = GetActorLocation();
		FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(Location, Location + DesiredMoveDirection);
		DesiredYaw = Rotator.Yaw;
	}
}
