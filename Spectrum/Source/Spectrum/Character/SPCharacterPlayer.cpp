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
#include "Potion/SPBlackPotion.h"
#include "Potion/SPGreenPotion.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/SceneComponent.h"
#include "Components/DecalComponent.h"
#include "Potion/SPOrangePotion.h"
#include "Potion/SPPurplePotion.h"
#include "SPCharacterMovementComponent.h"
#include "EngineUtils.h"
#include "SpectrumLog.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "Skill/SPSkillCastComponent.h"
#include "Skill/SPSlowSkill.h"
#include "SPGameState.h"
#include "Component/SPInventoryComponent.h"
#include "Player/SPPlayerController.h"
#include "Potion/SPItemBase.h"
#include "Potion/SPPickup.h"
#include "Skill/SPIceSkill.h"
#include "Skill/SPTeleSkill.h"
#include "UI/SPHUDWidget.h"
#include "Potion/SPSpectrumPotion.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Object/SPNonSimulateObject.h"
#include "Potion/Make/SPMakePotion.h"

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
	GetCapsuleComponent()->BodyInstance.bLockXRotation = true;
	GetCapsuleComponent()->BodyInstance.bLockYRotation = true;
	GetCapsuleComponent()->BodyInstance.bLockZRotation = true;


	//Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 350.0f;
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

	// //Skill
	SlowSkillComponent = CreateDefaultSubobject<USPSlowSkill>(TEXT("SkillComponent"));
	TeleSkillComponent = CreateDefaultSubobject<USPTeleSkill>(TEXT("TeleSkillComponent"));
	IceSkillComponent = CreateDefaultSubobject<USPIceSkill>(TEXT("IceSkillComponent"));

	SkillLocation = CreateDefaultSubobject<USceneComponent>(TEXT("SkillLocation"));
	SkillLocation->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshRef(
		TEXT("/Script/Engine.StaticMesh'/Engine/EditorMeshes/ArcadeEditorSphere.ArcadeEditorSphere'"));
	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	if (SphereMeshRef.Object)
	{
		Sphere->SetStaticMesh(SphereMeshRef.Object);
		Sphere->SetupAttachment(Staff);
		Sphere->SetWorldScale3D(FVector(-0.03125, -0.03125, -0.03125));
		Sphere->SetVisibility(false);
		Sphere->SetCollisionProfileName(TEXT("AllCollisionIgnore"));
	}


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

	// SkillLocation = CreateDefaultSubobject<USceneComponent>(TEXT("SkillLocation"));
	// SkillLocation->SetupAttachment(RootComponent);

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

	static ConstructorHelpers::FObjectFinder<UInputAction> InteractionKeyRef(TEXT(
		"/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_Interaction.IA_SP_Interaction'"));
	if (nullptr != InteractionKeyRef.Object)
	{
		InteractionKey = InteractionKeyRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> SlowQRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_SlowSkill.IA_SP_SlowSkill'"));
	if (nullptr != SlowQRef.Object)
	{
		SlowQ = SlowQRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IceERef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_IceSkill.IA_SP_IceSkill'"));
	if (nullptr != IceERef.Object)
	{
		IceE = IceERef.Object;
	}


	static ConstructorHelpers::FObjectFinder<UInputAction> TeleRRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_TeleSkill.IA_SP_TeleSkill'"));
	if (nullptr != TeleRRef.Object)
	{
		TeleR = TeleRRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> SpectrumFiveRef(
		TEXT(
			"/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_SpectrumPotionSpawn.IA_SP_SpectrumPotionSpawn'"));
	if (nullptr != SpectrumFiveRef.Object)
	{
		SpectrumFive = SpectrumFiveRef.Object;
	}


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

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MeshFinder1(
		TEXT("/Script/Engine.Material'/Game/Spectrum/Assets/Decal/M_Decal_Sphere_Black.M_Decal_Sphere_Black'"));
	if (MeshFinder1.Succeeded())
	{
		DecalSphere->SetMaterial(0, MeshFinder1.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MeshFinder2(
		TEXT("/Script/Engine.Material'/Game/Spectrum/Assets/Decal/M_Decal_Black.M_Decal_Black'"));
	if (MeshFinder2.Succeeded())
	{
		MyDecal->SetMaterial(0, MeshFinder2.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> KeyMenuRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_Key.IA_SP_Key'"));
	if (nullptr != KeyMenuRef.Object)
	{
		KeyMenu = KeyMenuRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> SevenKeyActionRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_Seven.IA_SP_Seven'"));
	if (nullptr != SevenKeyActionRef.Object)
	{
		SevenKeyAction = SevenKeyActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ChatEnterRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_Chat.IA_SP_Chat'"));
	if (nullptr != ChatEnterRef.Object)
	{
		ChatEnter = ChatEnterRef.Object;
	}

	PickupSound = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Spectrum/Sound/Pickup.Pickup'"));

	DecalSphere->SetVisibility(false);
	MyDecal->SetVisibility(false);

	CurrentCharacterControlType = ECharacterControlType::Shoulder;
	LastInput = FVector2D::ZeroVector;
	bIsAiming = false;
	bIsHolding = false;
	bIsSpawn = false;
	bIsThrowReady = false;
	bIsTurnReady = false;
	bIsTurnLeft = false;
	bIsTurnReady = false;
	bIsDamage = false;
	InteractionCheck = false;
	bIsActiveSlowSkill = true;
	bIsActiveIceSkill = true;
	bIsActiveTeleSkill = true;
	bIsActiveGraping = true;
	bSkillRunning = false;
	SphereRange = 100000;
	bIsPicking = false;
	KeyToggle = true;
	bIsSeven = false;
	bActiveHitIce = false;
	HitDistance = 1800.f;
	InteractionCheckFrequency = 0.1;
	InteractionCheckDistance = 225.0f;
	BaseEyeHeight = 74.0f;

	//Inventory
	PlayerInventory = CreateDefaultSubobject<USPInventoryComponent>(TEXT("playerInventory"));

	static ConstructorHelpers::FObjectFinder<USoundWave> GrapSoundRef(
		TEXT("/Script/Engine.SoundWave'/Game/Spectrum/Sound/GrapSound.GrapSound'"));
	if (GrapSoundRef.Object)
	{
		GrapSound = GrapSoundRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> StopGrapSoundRef(
		TEXT("/Script/Engine.SoundWave'/Game/Spectrum/Sound/StopGrapingSound.StopGrapingSound'"));
	if (StopGrapSoundRef.Object)
	{
		StopGrapSound = StopGrapSoundRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> SlowEffectRef(
		TEXT("/Script/Niagara.NiagaraSystem'/Game/Ice_Magic/VFX_Niagara/NS_Ice_Magic_Arena.NS_Ice_Magic_Arena'"));
	if (SlowEffectRef.Succeeded())
	{
		SlowEffect = SlowEffectRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> IceEffectRef(
		TEXT("/Script/Niagara.NiagaraSystem'/Game/Ice_Magic/VFX_Niagara/NS_Ice_Magic_Sheild.NS_Ice_Magic_Sheild'"));
	if (IceEffectRef.Succeeded())
	{
		IceEffect = IceEffectRef.Object;
	}


	bCanUseInput = true;
	bInteracionOnce = false;
}

void ASPCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}
	SetCharacterControl(CurrentCharacterControlType);
	if (GetMesh())
	{
		if (GetMesh()->GetAnimInstance())
		{
			GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(
				this, &ASPCharacterPlayer::HandleMontageAnimNotify);
		}
	}
};

void ASPCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsHolding && HasAuthority())
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

		EnhancedInputComponent->BindAction(SpectrumFive, ETriggerEvent::Triggered, this,
		                                   &ASPCharacterPlayer::SpectrumPotionSpawn);

		EnhancedInputComponent->BindAction(ThrowCtrl, ETriggerEvent::Triggered, this, &ASPCharacterPlayer::AimPotion);
		EnhancedInputComponent->BindAction(ThrowCtrl, ETriggerEvent::Completed, this, &ASPCharacterPlayer::ThrowPotion);

		EnhancedInputComponent->BindAction(GreenOne, ETriggerEvent::Triggered, this,
		                                   &ASPCharacterPlayer::GreenPotionSpawn);
		EnhancedInputComponent->BindAction(OrangeTwo, ETriggerEvent::Triggered, this,
		                                   &ASPCharacterPlayer::OrangePotionSpawn);
		EnhancedInputComponent->BindAction(PurpleThree, ETriggerEvent::Triggered, this,
		                                   &ASPCharacterPlayer::PurplePotionSpawn);
		EnhancedInputComponent->BindAction(InteractionKey, ETriggerEvent::Triggered, this,
		                                   &ASPCharacterPlayer::BeginInteract);
		EnhancedInputComponent->BindAction(SlowQ, ETriggerEvent::Triggered, this,
		                                   &ASPCharacterPlayer::SlowSKill);
		EnhancedInputComponent->BindAction(IceE, ETriggerEvent::Triggered, this,
		                                   &ASPCharacterPlayer::IceSKill);
		EnhancedInputComponent->BindAction(KeyMenu, ETriggerEvent::Triggered, this,
		                                   &ASPCharacterPlayer::ToggleKeyWidget);
		EnhancedInputComponent->BindAction(TeleR, ETriggerEvent::Triggered, this,
		                                   &ASPCharacterPlayer::TeleSKill);
		EnhancedInputComponent->BindAction(SevenKeyAction, ETriggerEvent::Triggered, this,
		                                   &ASPCharacterPlayer::SevenKey);
		EnhancedInputComponent->BindAction(ChatEnter, ETriggerEvent::Triggered, this,
		                                   &ASPCharacterPlayer::Chatting);
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

	ASPPlayerController* SPController = Cast<ASPPlayerController>(GetController());
	if (SPController)
	{
		HUDWidget = SPController->GetSPHUDWidget();
	}
	HUDWidget->UpdateMakingPotionWidget(false);
	HUDWidget->UpdateManualWidget(false);
	HUDWidget->ToggleMouse(false);
}

void ASPCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	if (!bCanUseInput)
	{
		return;
	}

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
	if (!bCanUseInput)
	{
		return;
	}
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);

		if (GetControlRotation().Yaw > PreControlYawRotation)
		{
			bIsTurnRight = true;
			bIsTurnLeft = false;
			if (!HasAuthority())
			{
				ServerRPCdirection(bIsTurnRight, bIsTurnLeft);
			}
		}
		else
		{
			bIsTurnRight = false;
			bIsTurnLeft = true;
			if (!HasAuthority())
			{
				ServerRPCdirection(bIsTurnRight, bIsTurnLeft);
			}
		}
		PreControlYawRotation = GetControlRotation().Yaw;
	}
}

void ASPCharacterPlayer::StopShoulderLook(const FInputActionValue& Value)
{
	if (!bCanUseInput)
	{
		return;
	}
	bIsTurnRight = false;
	bIsTurnLeft = false;
	if (!HasAuthority())
		ServerRPCdirection(bIsTurnRight, bIsTurnLeft);
}

void ASPCharacterPlayer::HandleMontageAnimNotify(FName NotifyName,
                                                 const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == FName("PlayMontageNotify"))
	{
		bIsThrowReady = true;
		if (IsLocallyControlled())
		{
			ShowProjectilePath();
		}
	}

	if (NotifyName == FName("SkillNotify"))
	{
		if (HasAuthority())
		{
			SlowSkillComponent->SkillAction();
		}
	}

	if (NotifyName == FName("IceSkillNotify"))
	{
		if (HasAuthority())
		{
			IceSkillComponent->SkillAction();
		}
	}

	if (NotifyName == FName("TeleSkillNotify"))
	{
		if (HasAuthority())
		{
			TeleSkillComponent->SkillAction();
		}
	}
}


void ASPCharacterPlayer::SpeedUp(const FInputActionValue& Value)
{
	if (false == bIsAiming && false == bIsHolding && false == bIsDamage)
	{
		if (!HasAuthority())
		{
			if (GetCharacterMovement())
			{
				GetCharacterMovement()->MaxWalkSpeed = 900.f;
			}
		}
		ServerRPCSpeedUp();
	}
}

void ASPCharacterPlayer::StopSpeedUp(const FInputActionValue& Value)
{
	if (!HasAuthority() && false == bIsDamage)
	{
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MaxWalkSpeed = 500.f;
		}
	}
	ServerRPCSpeedUpStop();
}

void ASPCharacterPlayer::Aiming(const FInputActionValue& Value)
{
	if (false == bIsHolding)
	{
		OnAimChanged.Broadcast(true);
		Aiming_CameraMove(); //애니메이션 작동
		ServerRPCAiming();
		bIsAiming = true;
		OnAimChanged.Broadcast(bIsAiming);
	}
}

void ASPCharacterPlayer::ServerRPCAiming_Implementation()
{
	//Test();
	bIsAiming = true;
	MultiRPCAimRotation(true);
}

void ASPCharacterPlayer::StopAiming(const FInputActionValue& Value)
{
	//bIsAiming = false;

	OnAimChanged.Broadcast(false);
	// GetCharacterMovement()->bOrientRotationToMovement = true;
	// GetCharacterMovement()->bUseControllerDesiredRotation = false;
	FollowCamera->K2_AttachToComponent(CameraBoom, NAME_None, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld,
	                                   EAttachmentRule::KeepWorld, true);
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld,
	                                          EAttachmentRule::KeepWorld, true);
	FollowCamera->AttachToComponent(CameraBoom, AttachmentRules, NAME_None);
	CameraMove();
	ServerRPCStopAiming();
}

void ASPCharacterPlayer::ServerRPCStopAiming_Implementation()
{
	MultiRPCAimRotation(false);
	bIsAiming = false;
}


void ASPCharacterPlayer::StopGraping(const FInputActionValue& Value)
{
	ServerRPCStopGraping();
}

void ASPCharacterPlayer::AimPotion(const FInputActionValue& Value)
{
	if (bIsSpawn && false == IsMontagePlaying())
	{
		if (!HasAuthority()) //클라이언트
		{
			bIsTurnReady = true;
			PlayTurnAnimation();
		}
		ServerRPCTurnReady();
	}
}

void ASPCharacterPlayer::ServerRPCTurnReady_Implementation()
{
	bIsTurnReady = true;
	PlayTurnAnimation();
	for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
	{
		if (PlayerController && GetController() != PlayerController) //시뮬레이트 프록시
		{
			if (!PlayerController->IsLocalController())
			{
				ASPCharacterPlayer* OtherPlayer = Cast<ASPCharacterPlayer>(PlayerController->GetPawn());
				if (OtherPlayer)
				{
					OtherPlayer->ClientRPCTurnAnimation(this);
				}
			}
		}
	}
}


void ASPCharacterPlayer::ThrowPotion(const FInputActionValue& Value)
{
	if (GetMesh() == nullptr || GetMesh()->GetAnimInstance() == nullptr) // 예외처리 
	{
		return;
	}

	if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(ThrowMontage))
	{
		if (bIsThrowReady) //던질 준비 완료
		{
			bIsTurnReady = false;
			ServerRPCThrowPotion(bIsThrowReady);
			if (!HasAuthority())
			{
				PlayThrowAnimation();

				bIsThrowReady = false;
				if (GetCharacterMovement())
				{
					GetCharacterMovement()->bOrientRotationToMovement = true;
					GetCharacterMovement()->bUseControllerDesiredRotation = false;
				}
				bIsTurnReady = false;
				bIsSpawn = false;
				Potion = nullptr;
			}
		}
		else //던질 준비가 안된상태이다. 
		{
			if (!HasAuthority())
			{
				PlayStopAnimation();
			}
			ServerRPCThrowPotion(bIsThrowReady);
		}
	}
}

void ASPCharacterPlayer::Jumping(const FInputActionValue& Value)
{
	if (!bCanUseInput)
	{
		return;
	}

	if (!bIsAiming)
	{
		bPressedJump = true;
		JumpKeyHoldTime = 0.0f;
	}
}

void ASPCharacterPlayer::MyStopJumping(const FInputActionValue& Value)
{
	if (!bCanUseInput)
	{
		return;
	}
	bPressedJump = false;
	ResetJumpState();
}

void ASPCharacterPlayer::BlackPotionSpawn(const FInputActionValue& Value)
{
	ServerRPCBlackPotionSpawn();
}

void ASPCharacterPlayer::GreenPotionSpawn(const FInputActionValue& Value)
{
	ServerRPCGreenPotionSpawn();
}

void ASPCharacterPlayer::OrangePotionSpawn(const FInputActionValue& Value)
{
	ServerRPCOrangePotionSpawn();
}

void ASPCharacterPlayer::PurplePotionSpawn(const FInputActionValue& Value)
{
	ServerRPCPurplePotionSpawn();
}

void ASPCharacterPlayer::SpectrumPotionSpawn(const FInputActionValue& Value)
{
	ServerRPCSpectrumPotionSpawn();
}

void ASPCharacterPlayer::ServerRPCSpectrumPotionSpawn_Implementation()
{
	if (GetMesh() == nullptr)
	{
		return;
	}
	if (PlayerInventory->CountPotion(PlayerInventory->IsPotion("S_Potion")) || bIsSeven)
	{
		if (false == bIsSpawn)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
			SpawnParams.Owner = this;
			Potion = GetWorld()->SpawnActor<ASPSpectrumPotion>(ASPSpectrumPotion::StaticClass(),
			                                                   GetMesh()->GetSocketLocation("Item_Socket"),
			                                                   FRotator{0.0f, 0.0f, 0.0f}, SpawnParams);
			bIsSpawn = true;
			if (Potion)
			{
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget,
				                                          EAttachmentRule::SnapToTarget,
				                                          EAttachmentRule::SnapToTarget, true);
				Potion->AttachToComponent(GetMesh(), AttachmentRules, FName{"Item_Socket"});
			}
		}
		if (!bIsSeven)
		{
			PlayerInventory->RemoveAmountOfItem(PlayerInventory->FindPotionItem("S_Potion"), 1);
		}
	}
}

// void ASPCharacterPlayer::Interaction(const FInputActionValue& Value)
// {
// 	TArray<AActor*> OverlappingActors;
// 	GetOverlappingActors(OverlappingActors);
//
// 	for (AActor* Actor : OverlappingActors)
// 	{
// 		//Todo if를 두번 해야하는건가?
// 		if (Actor->GetClass()->ImplementsInterface(USPGetInterface::StaticClass()))
// 		{
// 			ISPGetInterface* PotionActor = Cast<ISPGetInterface>(Actor);
// 			if (PotionActor)
// 			{
// 				PotionActor->GetPotion();
// 				break;
// 			}
// 		}
// 	}
// }

void ASPCharacterPlayer::ToggleKeyWidget(const FInputActionValue& Value)
{
	//HUDWidget->bIsShowMouse();
	if (KeyToggle)
	{
		HUDWidget->ToggleKeyWidget(false);
		KeyToggle = false;
	}
	else
	{
		HUDWidget->ToggleKeyWidget(true);
		KeyToggle = true;
	}
}

void ASPCharacterPlayer::IceSKill(const FInputActionValue& Value)
{
	if (!bCanUseInput)
	{
		return;
	}
	if (GetCharacterMovement() == nullptr)
	{
		return;
	}
	if ((GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking || GetCharacterMovement()->MovementMode ==
		EMovementMode::MOVE_None) && false == IsMontagePlaying())
	{
		ServerRPCIceSkill(GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
	}
}

void ASPCharacterPlayer::TeleSKill(const FInputActionValue& Value)
{
	if (!bCanUseInput)
	{
		return;
	}
	if (GetCharacterMovement() == nullptr)
	{
		return;
	}
	if ((GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking || GetCharacterMovement()->MovementMode ==
		EMovementMode::MOVE_None) && false == IsMontagePlaying())
	{
		ServerRPCTeleSkill(GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
	}
}


void ASPCharacterPlayer::ServerRPCTeleSkill_Implementation(float AttackStartTime)
{
	if (bIsActiveTeleSkill)
	{
		bIsActiveTeleSkill = false;
		TeleSkillComponent->ActivetedTimeStamp = GetWorld()->GetTime().GetWorldTimeSeconds();
		AttackTimeDifference = GetWorld()->GetTimeSeconds() - AttackStartTime;
		AttackTimeDifference = FMath::Clamp(AttackTimeDifference, 0.0f, SlowAttackTime - 0.01f);

		FTimerHandle Handle;
		if (GetCharacterMovement())
		{
			bSkillRunning = true;
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		}
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &ASPCharacterPlayer::SetMovementModeWalking,
		                                       1.5 - AttackTimeDifference, false, -1.0f);

		PlayTeleSkillAnimation();

		for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
		{
			ASPCharacterPlayer* OtherPlayer = Cast<ASPCharacterPlayer>(PlayerController->GetPawn());
			if (OtherPlayer)
			{
				OtherPlayer->ClientRPCTeleAnimation(this);
			}
		}
	}
}

void ASPCharacterPlayer::ServerRPCIceSkill_Implementation(float AttackStartTime)
{
	if (bIsActiveIceSkill)
	{
		bIsActiveIceSkill = false;
		IceSkillComponent->ActivetedTimeStamp = GetWorld()->GetTime().GetWorldTimeSeconds();
		AttackTimeDifference = GetWorld()->GetTimeSeconds() - AttackStartTime;
		AttackTimeDifference = FMath::Clamp(AttackTimeDifference, 0.0f, SlowAttackTime - 0.01f);

		if (GetCharacterMovement())
		{
			bSkillRunning = true;
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		}

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &ASPCharacterPlayer::SetMovementModeWalking,
		                                       2.3 - AttackTimeDifference, false, -1.0f);
		PlayIceSkillAnimation();

		for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
		{
			ASPCharacterPlayer* OtherPlayer = Cast<ASPCharacterPlayer>(PlayerController->GetPawn());
			if (OtherPlayer)
			{
				OtherPlayer->ClientRPCIceAnimation(this);
			}
		}
	}
}

void ASPCharacterPlayer::SlowSKill(const FInputActionValue& Value)
{
	if (!bCanUseInput)
	{
		return;
	}
	if (GetCharacterMovement() == nullptr)
	{
		return;
	}
	if ((GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking || GetCharacterMovement()->MovementMode ==
		EMovementMode::MOVE_None) && false == IsMontagePlaying())
	{
		ServerRPCSlowSkill(GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
	}
}


void ASPCharacterPlayer::ServerRPCSlowSkill_Implementation(float AttackStartTime)
{
	if (bIsActiveSlowSkill)
	{
		bIsActiveSlowSkill = false;
		SlowSkillComponent->ActivetedTimeStamp = GetWorld()->GetTime().GetWorldTimeSeconds();
		AttackTimeDifference = GetWorld()->GetTimeSeconds() - AttackStartTime;
		AttackTimeDifference = FMath::Clamp(AttackTimeDifference, 0.0f, SlowAttackTime - 0.01f);

		if (GetCharacterMovement())
		{
			bSkillRunning = true; //스킬 발동중 

			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		}

		FTimerHandle Handle;

		GetWorld()->GetTimerManager().SetTimer(Handle, this, &ASPCharacterPlayer::SetMovementModeWalking,
		                                       SlowAttackTime - AttackTimeDifference, false, -1.0f);
		//2.0f
		PlaySkillAnimation();

		for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
		{
			ASPCharacterPlayer* OtherPlayer = Cast<ASPCharacterPlayer>(PlayerController->GetPawn());
			if (OtherPlayer)
			{
				OtherPlayer->ClientRPCSlowAnimation(this);
			}
		}
	}
}

void ASPCharacterPlayer::ClientRPCSlowAnimation_Implementation(ASPCharacterPlayer* CharacterToPlay)
{
	if (CharacterToPlay)
	{
		CharacterToPlay->PlaySkillAnimation();
	}
}

void ASPCharacterPlayer::OnRep_Potion()
{
	if (GetMesh() == nullptr)
	{
		return;
	}
	if (Potion)
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
		                                          EAttachmentRule::SnapToTarget, true);
		Potion->AttachToComponent(GetMesh(), AttachmentRules, FName{"Item_Socket"});
	}
}

void ASPCharacterPlayer::ClientRPCThrowAnimation_Implementation(ASPCharacterPlayer* CharacterToPlay)
{
	if (CharacterToPlay)
	{
		CharacterToPlay->PlayThrowAnimation();
	}
}

void ASPCharacterPlayer::ClientRPCStopAnimation_Implementation(ASPCharacterPlayer* CharacterToPlay)
{
	if (CharacterToPlay)
	{
		CharacterToPlay->PlayStopAnimation();
	}
}


void ASPCharacterPlayer::ServerRPCGreenPotionSpawn_Implementation()
{
	if (GetMesh() == nullptr)
	{
		return;
	}

	if (PlayerInventory->CountPotion(PlayerInventory->IsPotion("G_Potion")) || bIsSeven)
	{
		if (false == bIsSpawn)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
			SpawnParams.Owner = this;
			Potion = GetWorld()->SpawnActor<ASPGreenPotion>(ASPGreenPotion::StaticClass(),
			                                                GetMesh()->GetSocketLocation("Item_Socket"),
			                                                FRotator{0.0f, 0.0f, 0.0f}, SpawnParams);
			bIsSpawn = true;

			if (Potion)
			{
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget,
				                                          EAttachmentRule::SnapToTarget,
				                                          EAttachmentRule::SnapToTarget, true);
				Potion->AttachToComponent(GetMesh(), AttachmentRules, FName{"Item_Socket"});
			}
		}
		if (!bIsSeven)
		{
			PlayerInventory->RemoveAmountOfItem(PlayerInventory->FindPotionItem("G_Potion"), 1);
		}
	}
}

void ASPCharacterPlayer::ServerRPCOrangePotionSpawn_Implementation()
{
	if (GetMesh() == nullptr)
	{
		return;
	}
	if (PlayerInventory->CountPotion(PlayerInventory->IsPotion("O_Potion")) || bIsSeven)
	{
		if (false == bIsSpawn)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
			SpawnParams.Owner = this;
			Potion = GetWorld()->SpawnActor<ASPOrangePotion>(ASPOrangePotion::StaticClass(),
			                                                 GetMesh()->GetSocketLocation("Item_Socket"),
			                                                 FRotator{0.0f, 0.0f, 0.0f}, SpawnParams);
			bIsSpawn = true;
			if (Potion)
			{
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget,
				                                          EAttachmentRule::SnapToTarget,
				                                          EAttachmentRule::SnapToTarget, true);
				Potion->AttachToComponent(GetMesh(), AttachmentRules, FName{"Item_Socket"});
			}
		}

		if (!bIsSeven)
		{
			PlayerInventory->RemoveAmountOfItem(PlayerInventory->FindPotionItem("O_Potion"), 1);
		}
	}
}

void ASPCharacterPlayer::ServerRPCPurplePotionSpawn_Implementation()
{
	if (GetMesh() == nullptr)
	{
		return;
	}

	if (PlayerInventory->CountPotion(PlayerInventory->IsPotion("P_Potion")) || bIsSeven)
	{
		if (false == bIsSpawn)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
			SpawnParams.Owner = this;
			Potion = GetWorld()->SpawnActor<ASPPurplePotion>(ASPPurplePotion::StaticClass(),
			                                                 GetMesh()->GetSocketLocation("Item_Socket"),
			                                                 FRotator{0.0f, 0.0f, 0.0f}, SpawnParams);
			bIsSpawn = true;
			if (Potion)
			{
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget,
				                                          EAttachmentRule::SnapToTarget,
				                                          EAttachmentRule::SnapToTarget, true);
				Potion->AttachToComponent(GetMesh(), AttachmentRules, FName{"Item_Socket"});
			}
		}
		if (!bIsSeven)
		{
			PlayerInventory->RemoveAmountOfItem(PlayerInventory->FindPotionItem("P_Potion"), 1);
		}
	}
}


void ASPCharacterPlayer::ClientRPCTurnAnimation_Implementation(ASPCharacterPlayer* CharacterToPlay)
{
	if (CharacterToPlay)
	{
		CharacterToPlay->PlayTurnAnimation();
	}
}


void ASPCharacterPlayer::ClientRPCIceAnimation_Implementation(ASPCharacterPlayer* CharacterToPlay)
{
	if (CharacterToPlay)
	{
		CharacterToPlay->PlayIceSkillAnimation();
	}
}

void ASPCharacterPlayer::ServerRPCDragItem_Implementation(int Num, const int32 QuantityToDrop)
{
	USPItemBase* ItemBase = PlayerInventory->FindMatchingMiniItem(Num); //미니 물약에서 정보 찾아오기

	PlayerInventory->RemoveAmountOfItem(ItemBase, 1); //물약 하나 뺴기
	GetInventory()->AddInventorMakeContents(ItemBase);
	if (GetInventory()->GetInventorMakeContents().Num() == 3) //3개의 칸에 어떤게 들어왔는지 확인 GetInventorMakeContents
	{
		USPItemBase* Item = PlayerInventory->MakingPotion(); //만들 수 있는지 검사 
		if (Item)
		{
			int MakeNum = PlayerInventory->IsPotion(Item->ID);
			ClientRPCUpdateMakingPotion(MakeNum);
			PlayerInventory->ClearMakeArray();
		}
	}
}

void ASPCharacterPlayer::ClientRPCUpdateMakingPotion_Implementation(int Num)
{
	USPItemBase* ItemBase = PlayerInventory->FindMatchingItem(Num);
	HUDWidget->MakingPotionWieget(ItemBase);
}

void ASPCharacterPlayer::ServerRPCBackItem_Implementation(int Num, const int32 QuantityToDrop)
{
	USPItemBase* ItemBase = PlayerInventory->FindMatchingMiniItem(Num);

	PlayerInventory->HandleAddItem(ItemBase, 1); //미니 인벤토리에 넣는 것
	GetInventory()->RemoveInventorMakeContents(ItemBase); //3개에 배열 있는 것에서 지워주는 것 
}

void ASPCharacterPlayer::ServerRPCAddItemClick_Implementation(int Num)
{
	USPItemBase* ItemBase = PlayerInventory->FindMatchingItem(Num);
	PlayerInventory->HandleAddItem(ItemBase, 1);
}


void ASPCharacterPlayer::ClientRPCTeleAnimation_Implementation(ASPCharacterPlayer* CharacterToPlay)
{
	if (CharacterToPlay)
	{
		CharacterToPlay->PlayTeleSkillAnimation();
	}
}


void ASPCharacterPlayer::ServerRPCWidgetMove_Implementation(bool Move)
{
	MultiRPCWidgetMove(Move);
}

void ASPCharacterPlayer::MultiRPCWidgetMove_Implementation(bool move)
{
	if (move)
	{
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
		}
	}
	else
	{
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MovementMode = EMovementMode::MOVE_None;
		}
	}
}

void ASPCharacterPlayer::PlayTurnAnimation()
{
	if (GetMesh())
	{
		if (GetMesh()->GetAnimInstance())
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			AnimInstance->Montage_Play(ThrowMontage, 1.0f);
			// TorsoAnimInstance->Montage_Play(ThrowMontage, 1.0f);
			if (GetCharacterMovement())
			{
				GetCharacterMovement()->bOrientRotationToMovement = false;
				GetCharacterMovement()->bUseControllerDesiredRotation = true;
			}
		}
	}
}

void ASPCharacterPlayer::PlayThrowAnimation()
{
	if (GetMesh())
	{
		if (GetMesh()->GetAnimInstance())
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			AnimInstance->Montage_JumpToSection(FName("End"), ThrowMontage);
		}
	}
}

void ASPCharacterPlayer::PlayStopAnimation()
{
	if (GetMesh() == nullptr || GetMesh()->GetAnimInstance() == nullptr)
	{
		return;
	}
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	UAnimInstance* TorsoAnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Stop(0.0f);
}

void ASPCharacterPlayer::ServerRPCdirection_Implementation(bool TurnRight, bool Turnleft)
{
	bIsTurnRight = TurnRight;
	bIsTurnLeft = Turnleft;
}

void ASPCharacterPlayer::ServerRPCThrowPotion_Implementation(bool IsThrowReady)
{
	if (IsThrowReady)
	{
		PlayThrowAnimation();
		if (Potion)
		{
			//GetController()->GetControlRotation();
			FVector ForwardVector = UKismetMathLibrary::GetForwardVector(GetController()->GetControlRotation());
			float Mul = 1500.0f;
			Potion->Throw((ForwardVector + FVector{0.0f, 0.0f, 0.4f}) * Mul);
		}
		bIsTurnReady = false;
		bIsThrowReady = false;
		MultiRPCAimRotation(false);
		bIsSpawn = false;
		Potion = nullptr;
		for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
		{
			if (PlayerController && GetController() != PlayerController) //시뮬레이트 프록시
			{
				if (!PlayerController->IsLocalController())
				{
					//서버 아니고 공격 명령 내린 플레이어 컨트롤러도 아닌 시뮬레이트 프록시
					//폰을 재생하는 플레이어 컨트롤러
					ASPCharacterPlayer* OtherPlayer = Cast<ASPCharacterPlayer>(PlayerController->GetPawn());
					if (OtherPlayer)
					{
						OtherPlayer->ClientRPCThrowAnimation(this);
					}
				}
			}
		}
	}
	else //던질 준비가 안된 상태이다. 
	{
		bIsTurnReady = false; //턴인플레이스 멈추기
		MultiRPCAimRotation(false);
		PlayStopAnimation();
		for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
		//플레이어 컨트롤러 목록을 서버에서 가지고 오기
		{
			if (PlayerController && GetController() != PlayerController) //시뮬레이트 프록시
			{
				if (!PlayerController->IsLocalController())
				{
					ASPCharacterPlayer* OtherPlayer = Cast<ASPCharacterPlayer>(PlayerController->GetPawn());
					if (OtherPlayer)
					{
						OtherPlayer->ClientRPCStopAnimation(this);
					}
				}
			}
		}
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
		FVector LaunchVelocity = (UKismetMathLibrary::GetForwardVector(GetController()->GetControlRotation())
			+ FVector{0.0f, 0.0f, 0.4f}) * 1500.0f;
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

		DecalSphere->SetVisibility(true);
		MyDecal->SetVisibility(true);
		DecalSphere->SetWorldLocation(OutHit.Location, false, &SweepHitResult, ETeleportType::TeleportPhysics);

		for (int i = 0; i < OutPathPositions.Num(); i++)
		{
			Projectile_Path->AddSplinePointAtIndex(OutPathPositions[i], i, ESplineCoordinateSpace::Type::Local,
			                                       true);
		}
		for (int i = 0; i < Projectile_Path->GetNumberOfSplinePoints() - 1; ++i)
		{
			UClass* whyClass = USplineMeshComponent::StaticClass();
			FTransform RelativeTransform = FTransform();

			USplineMeshComponent* NewSplineMeshComp = NewObject<USplineMeshComponent>(
				this, USplineMeshComponent::StaticClass());

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
			NewSplineMeshComp->SetStaticMesh(StaticMeshforSpline);
			NewSplineMeshComp->SetMobility(EComponentMobility::Movable);
			NewSplineMeshComp->SetCollisionProfileName(TEXT("SplineCollision"));
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
		DecalSphere->SetVisibility(false);
		MyDecal->SetVisibility(false);
	}
}

void ASPCharacterPlayer::SetupHUDWidget(USPHUDWidget* InUserWidget)
{
	SlowSkillComponent->OnSlowCDChange.AddUObject(InUserWidget, &USPHUDWidget::UpdateSlowCDTime);
	IceSkillComponent->OnIceCDChange.AddUObject(InUserWidget, &USPHUDWidget::UpdateIceCDTime);
	TeleSkillComponent->OnTeleCDChange.AddUObject(InUserWidget, &USPHUDWidget::UpdateTeleCDTime);

	OnAimChanged.AddUObject(InUserWidget, &USPHUDWidget::UpdateTarget);
	AGameStateBase* State = GetController()->GetWorld()->GetGameState();
	if (State)
	{
		ASPGameState* SPGameState = Cast<ASPGameState>(State);
		if (SPGameState)
		{
			SPGameState->OnScore.AddUObject(InUserWidget, &USPHUDWidget::UpdateScore);
			SPGameState->OnTime.AddUObject(InUserWidget, &USPHUDWidget::UpdateTime);
		}
	}
}

void ASPCharacterPlayer::UpdateItemData(AActor* InActor) //아이템 정보를 받음 
{
	if (InActor->Implements<USPInteractionInterface>()) // 상호작용 가능한 액터인지 검사한다. 
	{
		FoundInteractable(InActor);
		bInteracionOnce = false;
		return;
	}
	ClearItemData(); // 상호작용할 수 있는 액터를 찾지 못했을 때의 처리를 수행
}

void ASPCharacterPlayer::FoundInteractable(AActor* NewInteractable) //상호작용 가능한 것이라면 데이터를 갱신해준다. 
{
	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;
	ClientRPCInteractionPotionUI(NewInteractable);
}

void ASPCharacterPlayer::ClientRPCInteractionPotionUI_Implementation(AActor* NewInteractable)
{
	TargetInteractable = NewInteractable;
	bInteracionOnce = false;
	if (HUDWidget)
	{
		HUDWidget->UpdateInteractionWidget(&TargetInteractable->InteractableData); //위젯을 띄운다. 
	}
}


void ASPCharacterPlayer::ClearItemData() // 트리거에서 나간 경우 호출되는 함수 
{
	TargetInteractable = nullptr;
	bInteracionOnce = false;
	ClientRPCHideInteractionUI();
}

void ASPCharacterPlayer::ClientRPCHideInteractionUI_Implementation()
{
	if (HUDWidget)
	{
		HUDWidget->HideInteractionWidget();
	}
	// 인터렉션 정보 없앤다. 
}

void ASPCharacterPlayer::ClientRPCInteractionWidget_Implementation()
{
	if (Cast<ASPMakePotion>(TargetInteractable.GetObject()))
	{
		HUDWidget->UpdateMakingPotionWidget(true);
		HUDWidget->ToggleMouse(true);
	}
	else
	{
		HUDWidget->UpdateManualWidget(true);
		HUDWidget->ToggleMouse(true);
	}
}

void ASPCharacterPlayer::BeginInteract() //f 키를 누르면 호출
{
	// if (InteractionData.CurrentInteractable)
	// {
	// 	SP_LOG(LogTemp,Log,TEXT("BeginInteract"));
	SP_LOG(LogTemp, Log, TEXT("BeginInteract"));

	//if (IsValid(TargetInteractable.GetObject()) && !bInteracionOnce)
	SP_LOG(LogTemp, Log, TEXT("bInteracionOnce = %d "), bInteracionOnce);
	SP_LOG(LogTemp, Log, TEXT("TargetInteractable = %d "), IsValid(TargetInteractable.GetObject()));

	if (IsValid(TargetInteractable.GetObject()) && !bInteracionOnce)
	{
		SP_LOG(LogTemp, Log, TEXT("bInteracionOnce"));
		Interact();
		bInteracionOnce = true;
	}
	// }
}

void ASPCharacterPlayer::Interact()
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		if (ASPPickup* Pickup = Cast<ASPPickup>(TargetInteractable.GetObject())) //인터렉션까지 들어왔는데 포션과 인터렉션인 경우?
		{
			if (IsMontagePlaying() == false)
			{
				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupSound, GetActorLocation(),
					                                      GetActorRotation());
				}, 0.8f, false);
				ServerRPCInteract();
			}
		}
		else //인터렉션까지 들어왔는데 제작대 &  인터렉션인 경우 ? 서버 Interact로 갈 필요가 없다. 
		{
			if (IsValid(TargetInteractable.GetObject()))
			{
				ClientRPCInteractionWidget();
			}
		}
	}
}

void ASPCharacterPlayer::ServerRPCInteract_Implementation()
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact(this, HUDWidget);
	}
	bIsPicking = true; //애니메이션 작동한다.

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASPCharacterPlayer::InteractionTimerFun, 1.5f, false);
}

void ASPCharacterPlayer::UpdateInteractionWidget() const
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		HUDWidget->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}
}

void ASPCharacterPlayer::AddItemClick(int Num)
{
	ServerRPCAddItemClick(Num);
}


void ASPCharacterPlayer::DragItem(USPItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	int num = PlayerInventory->IsMiniPotion(ItemToDrop->ID);
	ServerRPCDragItem(num, 1);
}

bool ASPCharacterPlayer::BackItem(USPItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	if (PlayerInventory->IsMiniPotion(ItemToDrop->ID) == -1)
	{
		return false;
	}
	int num = PlayerInventory->IsMiniPotion(ItemToDrop->ID);
	ServerRPCBackItem(num, QuantityToDrop);
	return true;
}


void ASPCharacterPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASPCharacterPlayer, bIsSpawn);
	DOREPLIFETIME(ASPCharacterPlayer, bIsAiming);
	DOREPLIFETIME(ASPCharacterPlayer, Potion);
	DOREPLIFETIME(ASPCharacterPlayer, bIsTurnLeft);
	DOREPLIFETIME(ASPCharacterPlayer, bIsTurnRight);
	DOREPLIFETIME(ASPCharacterPlayer, bIsTurnReady);
	DOREPLIFETIME(ASPCharacterPlayer, bIsThrowReady);
	DOREPLIFETIME(ASPCharacterPlayer, bIsHolding);
	DOREPLIFETIME(ASPCharacterPlayer, bIsPicking);
	DOREPLIFETIME(ASPCharacterPlayer, InteractionCheck);
	DOREPLIFETIME(ASPCharacterPlayer, bIsActiveGraping);
	DOREPLIFETIME(ASPCharacterPlayer, HitMyActor);
	DOREPLIFETIME(ASPCharacterPlayer, bCanUseInput);
	DOREPLIFETIME(ASPCharacterPlayer, bInteracionOnce);
	//bInteracionOnce
	//DOREPLIFETIME(ASPCharacterPlayer, TargetInteractable);
	//HitMyActor
	//DOREPLIFETIME(ASPCharacterPlayer, PhysicsHandleComponent);
	//PhysicsHandleComponent
	//DOREPLIFETIME(ASPCharacterPlayer, PhysicsHandleComponent);
	// DOREPLIFETIME(ASPCharacterPlayer, bIsActiveSlowSkill);
	//bIsActiveSlowSkill
	// DOREPLIFETIME(ASPCharacterPlayer, bIsActiveSlowSkill);
	// DOREPLIFETIME(ASPCharacterPlayer, SkillCastComponent);
	//SkillCastComponent
}

void ASPCharacterPlayer::PlaySkillAnimation()
{
	if (GetMesh())
	{
		if (GetMesh()->GetAnimInstance())
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			AnimInstance->StopAllMontages(0.0f);
			AnimInstance->Montage_Play(SkillMontage);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SlowSkillComponent->SkillSound, GetActorLocation(),
			                                      GetActorRotation());
		}
	}
}


void ASPCharacterPlayer::PlayIceSkillAnimation()
{
	if (GetMesh())
	{
		if (GetMesh()->GetAnimInstance())
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			AnimInstance->StopAllMontages(0.0f);
			AnimInstance->Montage_Play(SkillIceMontage);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), IceSkillComponent->SkillSound, GetActorLocation(),
			                                      GetActorRotation());
		}
	}
}

void ASPCharacterPlayer::PlayTeleSkillAnimation()
{
	if (GetMesh())
	{
		if (GetMesh()->GetAnimInstance())
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			AnimInstance->StopAllMontages(0.0f);
			AnimInstance->Montage_Play(SkillTeleMontage);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), TeleSkillComponent->SkillSound, GetActorLocation(),
			                                      GetActorRotation());
		}
	}
}

void ASPCharacterPlayer::HitSlowSkillResult()
{
	//todo 
	if (GetMesh() == nullptr || GetMesh()->GetAnimInstance() == nullptr)
	{
		return;
	}
	bIsDamage = true;
	FVector CapsuleRadius = FVector{0.0f, 0.0f, GetCapsuleComponent()->GetScaledCapsuleRadius() * 2};
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SlowEffect, GetActorLocation() - CapsuleRadius,
	                                               GetActorRotation());
	if (false == IsMontagePlaying())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(ImpactMontage, 1.0f);
	}
	GetCharacterMovement()->MaxWalkSpeed = 100.f;
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &ASPCharacterPlayer::SlowSillApply, 3, false, -1.0f);
}

void ASPCharacterPlayer::SlowSillApply()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	bIsDamage = false;
}

void ASPCharacterPlayer::SetMovementModeWalking() //스킬을 쓸 때 발동된다. 
{
	if (GetCharacterMovement())
	{
		bSkillRunning = false;

		if (!bActiveHitIce)
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		}
		//bIsDamage = false;
	}
}

void ASPCharacterPlayer::SetMovementModeSkillWalking() //맞았을 때 
{
	bActiveHitIce = false;
	if (GetCharacterMovement())
	{
		if (!bSkillRunning)
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		}
		bIsDamage = false;
	}
}

void ASPCharacterPlayer::InteractionTimerFun()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		bIsPicking = false;
	}
}


void ASPCharacterPlayer::HitIceSkillResult()
{
	if (GetMesh() == nullptr || GetMesh()->GetAnimInstance() == nullptr)
	{
		return;
	}
	bIsDamage = true;
	bActiveHitIce = true;
	FVector CapsuleRadius = FVector{0.0f, 0.0f, GetCapsuleComponent()->GetScaledCapsuleRadius() * 2};
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), IceEffect, GetActorLocation() - CapsuleRadius,
	                                               GetActorRotation());
	if (false == IsMontagePlaying())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(ImpactMontage, 1.0f);
	}
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &ASPCharacterPlayer::SetMovementModeSkillWalking, 4, false,
	                                       -1.0f);
}

void ASPCharacterPlayer::HitTeleSkillResult(const FVector TeleportLocation)
{
	if (GetMesh() == nullptr || GetMesh()->GetAnimInstance() == nullptr)
	{
		return;
	}
	if (false == IsMontagePlaying())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(ImpactMontage, 1.0f);
	}
	this->TeleportTo(TeleportLocation, this->GetActorRotation(), false, true);
}

void ASPCharacterPlayer::OverlapPortal(const FVector& Location)
{
	FTimerHandle Handle;
	if (GetCharacterMovement() == nullptr)
	{
		return;
	}
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
		                                       {
			                                       GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			                                       this->SetActorRelativeLocation(Location);
		                                       }
	                                       ), 5.0f, false);
}

void ASPCharacterPlayer::ActiveGrapping(const bool Active)
{
	bIsActiveGraping = Active;
}

bool ASPCharacterPlayer::IsMontagePlaying()
{
	if (GetMesh() == nullptr || GetMesh()->GetAnimInstance() == nullptr)
	{
		return false;
	}
	if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(ThrowMontage) ||
		GetMesh()->GetAnimInstance()->Montage_IsPlaying(SkillMontage) ||
		GetMesh()->GetAnimInstance()->Montage_IsPlaying(SkillIceMontage) ||
		GetMesh()->GetAnimInstance()->Montage_IsPlaying(SkillTeleMontage) ||
		GetMesh()->GetAnimInstance()->Montage_IsPlaying(ImpactMontage))

	{
		return true; //어떤 애니메이션 하나라도 플레이 중이면 트루 
	}
	else
	{
		return false;
	}
}


void ASPCharacterPlayer::NetTESTRPCSlowSkill_Implementation()
{
	if (GetCharacterMovement() == nullptr)
	{
		return;
	}
	//todo
	GetCharacterMovement()->MaxWalkSpeed = 100.f;
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
		                                       {
			                                       GetCharacterMovement()->MaxWalkSpeed = 500.f;
		                                       }
	                                       ), 5, false, -1.0f);
}


void ASPCharacterPlayer::ServerRPCBlackPotionSpawn_Implementation()
{
	if (GetMesh() == nullptr || GetMesh()->GetAnimInstance() == nullptr)
	{
		return;
	}
	if (PlayerInventory->CountPotion(PlayerInventory->IsPotion("B_Potion")) || bIsSeven)
	{
		if (false == bIsSpawn) //스폰이 되지 않았다면 ? 
		{
			FVector ItemLocation = GetMesh()->GetSocketLocation("Item_Socket");
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
			SpawnParams.Owner = this;
			Potion = GetWorld()->SpawnActor<ASPBlackPotion>(ASPBlackPotion::StaticClass(),
			                                                ItemLocation,
			                                                FRotator::ZeroRotator, SpawnParams);
			bIsSpawn = true;
			// if (Potion)
			// {
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget,
			                                          EAttachmentRule::SnapToTarget,
			                                          EAttachmentRule::SnapToTarget, true);
			Potion->AttachToComponent(GetMesh(), AttachmentRules, FName{"Item_Socket"});
			// }
		}
		if (!bIsSeven)
		{
			PlayerInventory->RemoveAmountOfItem(PlayerInventory->FindPotionItem("B_Potion"), 1);
		}
	}
}

void ASPCharacterPlayer::MultiChangeCollision_Implementation(const FName& CollisionName)
{
	if (HitMyActor) //내가 들고 있는 액터가 있는 것이면 ? 
	{
		ASPObject* MyObject = Cast<ASPObject>(HitMyActor);
		MyObject->SetObjectCollisionType(CollisionName);
	}
}


void ASPCharacterPlayer::Graping(const FInputActionValue& Value)
{
	if (!bCanUseInput)
	{
		return;
	}
	ServerRPCGraping();
}


void ASPCharacterPlayer::ServerRPCGraping_Implementation()
{
	MultiRPCAimRotation(false);
	if (false == bIsHolding)
	{
		FVector SphereLocationStart = FollowCamera->K2_GetComponentLocation();
		APlayerController* PlayerController = GetController<APlayerController>();
		FVector Location;
		FRotator Rotation;
		PlayerController->GetPlayerViewPoint(Location, Rotation);

		if (PlayerController != nullptr)
		{
			FRotator ControlRotation = PlayerController->GetControlRotation();
			FVector ReseltFoward = UKismetMathLibrary::GetForwardVector(ControlRotation);

			FVector WorldLocation;
			FVector WorldDirection;
			bool TransSuccess = PlayerController->DeprojectScreenPositionToWorld(
				0.5, 0.5, WorldLocation, WorldDirection);

			FVector SphereLocationEnd = Location + Rotation.Vector() * SphereRange;
			TArray<TEnumAsByte<EObjectTypeQuery>> EmptyObjectTypes;
			EDrawDebugTrace::Type drawDebugType = EDrawDebugTrace::None;
			TArray<AActor*> HitActorsToIgnore;
			TArray<AActor*> HitTriggerToIgnore;


			FLinearColor RedColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
			FLinearColor GreenColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);
			FCollisionQueryParams Params;
			//모든 캐릭터 타입은 무시하도록 하자,
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), HitActorsToIgnore);
			for (AActor* FoundActor : HitActorsToIgnore)
			{
				Params.AddIgnoredActor(FoundActor);
			}
			bool HitSuccess = GetWorld()->LineTraceSingleByChannel(outHitResult, Location, SphereLocationEnd,
			                                                       ECC_GameTraceChannel1, Params);
			if (HitSuccess && outHitResult.Component->Mobility == EComponentMobility::Movable)
			{
				outHitResult.Component->SetSimulatePhysics(true);
				outHitResult.GetActor()->SetOwner(this);
				HitComponent = outHitResult.GetComponent();
				HitMyActor = outHitResult.GetActor();

				if (Cast<ASPObject>(HitMyActor))
				{
					MultiChangeCollision(TEXT("GrapObjectCollision"));
				}


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
					EDrawDebugTrace::None, OutHits, true, GreenColor1, RedColor1, DrawTime1);

				ActorPrimitiveArray.Empty();

				if (Success)
				{
					for (const FHitResult& HitResult : OutHits)
					{
						AActor* Hit = HitResult.GetActor();

						ASPNonSimulateObject* SimulableObject = Cast<ASPNonSimulateObject>(Hit);
						if (SimulableObject)
						{
							SimulableObject->StartPhysicsSleepTimer();
						}


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
					ClientRPCSound(GrapSound);
				}
			}
		}
	}
	else
	{
		bIsHolding = false;
		if (HitComponent && HitComponent->IsSimulatingPhysics())
		{
			ClientRPCSound(StopGrapSound);
			PhysicsHandleComponent->ReleaseComponent();
			HitComponent->AddImpulse(FollowCamera->GetForwardVector() * HitDistance, NAME_None, true);
			if (Cast<ASPObject>(HitMyActor))
			{
				MultiChangeCollision(TEXT("PropCollision"));
			}
			HitComponent = nullptr;
		}
	}
}

void ASPCharacterPlayer::ServerRPCStopGraping_Implementation()
{
	if (bIsHolding && HitComponent->IsSimulatingPhysics())
	{
		bIsHolding = false;
		ClientRPCSound(StopGrapSound);
		PhysicsHandleComponent->ReleaseComponent();
		HitComponent->AddImpulse(FollowCamera->GetForwardVector() * HitDistance, NAME_None, true);
		if (Cast<ASPObject>(HitMyActor))
		{
			MultiChangeCollision(TEXT("PropCollision"));
		}
		HitComponent = nullptr;
	}
}

void ASPCharacterPlayer::Aiming_CameraMove()
{
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld,
	                                          EAttachmentRule::KeepWorld, true);
	if (false == bIsHolding)
	{
		FollowCamera->AttachToComponent(SpringArm, AttachmentRules, NAME_None);
		CameraMove();
	}
	else
	{
		FollowCamera->AttachToComponent(CameraBoom, AttachmentRules, NAME_None);
		CameraMove();
	}
}

void ASPCharacterPlayer::ServerRPCSpeedUpStop_Implementation()
{
	if (false == bIsDamage && GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
	}
}

void ASPCharacterPlayer::ServerRPCSpeedUp_Implementation()
{
	if (false == bIsDamage && GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 900.f;
	}
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

void ASPCharacterPlayer::SevenKey(const FInputActionValue& Value)
{
	//bIsSeven = true;
	ServerRPCSeven();
}


void ASPCharacterPlayer::ServerRPCSeven_Implementation()
{
	bIsSeven = true;
}

void ASPCharacterPlayer::ShowTargetUI(bool ShowUI)
{
	OnAimChanged.Broadcast(ShowUI);
}

void ASPCharacterPlayer::MultiRPCAimRotation_Implementation(bool IsAim)
{
	if (GetCharacterMovement() == nullptr)
	{
		return;
	}
	if (IsAim)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
	}
}

void ASPCharacterPlayer::ClientRPCSound_Implementation(USoundWave* Sound)
{
	if (Sound == GrapSound)
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld,
		                                          EAttachmentRule::KeepWorld, true);
		FollowCamera->AttachToComponent(CameraBoom, AttachmentRules, NAME_None);
		CameraMove();
	}
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetActorLocation(), GetActorRotation());
}

void ASPCharacterPlayer::Chatting(const FInputActionValue& Value)
{
	if (!bCanUseInput)
	{
		return;
	}
	HUDWidget->ShowChat();
}
