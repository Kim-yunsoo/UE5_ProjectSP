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
#include "MovieSceneSequenceID.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "Skill/SPSkillCastComponent.h"
#include "Skill/SPSlowSkill.h"
#include "UI/SPWidgetComponent.h"
#include "UI/SPTargetUI.h"
#include "DrawDebugHelpers.h"
#include "Component/SPInventoryComponent.h"
#include "Player/SPPlayerController.h"
#include "Potion/SPItemBase.h"
#include "Skill/SPIceSkill.h"
#include "UI/SPHUDWidget.h"
#include "UI/Inventory/SPPickup.h"


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

	// //Skill
	SlowSkillComponent = CreateDefaultSubobject<USPSlowSkill>(TEXT("SkillComponent"));

	IceSkillComponent = CreateDefaultSubobject<USPIceSkill>(TEXT("IceSkillComponent"));
	// SlowSkillComponent->SetIsReplicated(true);
	// SlowSkillComponent = CreateDefaultSubobject<USPSlowSkill>(TEXT("SlowSkill"));

	SkillLocation = CreateDefaultSubobject<USceneComponent>(TEXT("SkillLocation"));
	SkillLocation->SetupAttachment(RootComponent);

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

	static ConstructorHelpers::FObjectFinder<UInputAction> ToggleMenuRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_Inventory.IA_SP_Inventory'"));
	if (nullptr != ToggleMenuRef.Object)
	{
		ToggleMenu = ToggleMenuRef.Object;
	}
	//Effect

	// static ConstructorHelpers::FObjectFinder<UParticleSystem> SlowEffectRef(
	// 	TEXT("/Script/Engine.ParticleSystem'/Game/Box/MagicStaff/Demo/Particles/P_Explosion.P_Explosion'"));
	// if (SlowEffectRef.Object)
	// {
	// 	SlowEffect = SlowEffectRef.Object;
	// }

	//SlowSkillComponent->SetIsReplicated(true);

	DecalSphere->SetVisibility(false);
	MyDecal->SetVisibility(false);
	// DecalSphere->SetIsReplicated(true);

	//Widget
	//Target = CreateDefaultSubobject<USPWidgetComponent>(TEXT("Widget"));


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
	bIsDamage = false;

	bIsActiveSlowSkill = true;
	bIsActiveIceSkill = true;

	// bIsActiveSlowSkill = true;
	HitDistance = 1800.f;

	InteractionCheckFrequency = 0.1;
	InteractionCheckDistance = 225.0f;

	BaseEyeHeight = 74.0f;


	//Inventory
	PlayerInventory = CreateDefaultSubobject<USPInventoryComponent>(TEXT("playerInventory"));
	this->AddOwnedComponent(PlayerInventory);
	PlayerInventory->SetSlotsCapacity(10);
	PlayerInventory->SetWeightCapacity(50.f); //무게 의미 없음!
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
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(
		this, &ASPCharacterPlayer::HandleMontageAnimNotify);

	//
};

void ASPCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsHolding)
	{
		PhysicsHandleComponent->SetTargetLocation(GravityArrow->K2_GetComponentLocation());
	}

	//
	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
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
		EnhancedInputComponent->BindAction(InteractionKey, ETriggerEvent::Triggered, this,
		                                   &ASPCharacterPlayer::BeginInteract);
		EnhancedInputComponent->BindAction(InteractionKey, ETriggerEvent::Completed, this,
		                                   &ASPCharacterPlayer::EndInteract);

		EnhancedInputComponent->BindAction(SlowQ, ETriggerEvent::Triggered, this,
		                                   &ASPCharacterPlayer::SlowSKill);
		EnhancedInputComponent->BindAction(IceE, ETriggerEvent::Triggered, this,
		                                   &ASPCharacterPlayer::IceSKill);
		EnhancedInputComponent->BindAction(ToggleMenu, ETriggerEvent::Triggered, this,
		                                   &ASPCharacterPlayer::ToggleMenuAction);
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
	HUDWidget->bIsMenuVisible = true;
	HUDWidget->ToggleMenu();
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
		ShowProjectilePath();
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
			// SlowSkillComponent->SkillAction();
		}
	}
}


void ASPCharacterPlayer::SpeedUp(const FInputActionValue& Value)
{
	if (false == bIsAiming && false == bIsHolding && false == bIsDamage)
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
	if (!HasAuthority() && false == bIsDamage)
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
		ServerRPCAiming();
		bIsAiming = true;
		UE_LOG(LogTemp, Log, TEXT("Aiming"));
		OnAimChanged.Broadcast(bIsAiming);
	}
}

void ASPCharacterPlayer::ServerRPCAiming_Implementation()
{
	bIsAiming = true;
}

void ASPCharacterPlayer::StopAiming(const FInputActionValue& Value)
{
	bIsAiming = false;

	UE_LOG(LogTemp, Log, TEXT("StopAiming"));
	OnAimChanged.Broadcast(bIsAiming);
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
	ServerRPCGraping();
}

void ASPCharacterPlayer::StopGraping(const FInputActionValue& Value)
{
	ServerRPCStopGraping();
}

void ASPCharacterPlayer::AimPotion(const FInputActionValue& Value)
{
	if (bIsSpawn)
	{
		if (!HasAuthority())
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
	//플레이어 컨트롤러 목록을 서버에서 가지고 오기
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
					OtherPlayer->ClientRPCTurnAnimation(this);
				}
			}
		}
	}
}


void ASPCharacterPlayer::ThrowPotion(const FInputActionValue& Value)
{
	if (bIsThrowReady)
	{
		ServerRPCThrowPotion(bIsThrowReady);
		if (!HasAuthority())
		{
			PlayThrowAnimation();
			bIsThrowReady = false;
			GetCharacterMovement()->bOrientRotationToMovement = true;
			GetCharacterMovement()->bUseControllerDesiredRotation = false;
			bIsTurnReady = false;
			bIsSpawn = false;
			Potion = nullptr;
		}
		bIsTurnReady = false;
	}
	else
	{
		if (!HasAuthority())
		{
			PlayStopAnimation();
		}
		ServerRPCThrowPotion(bIsThrowReady);
	}
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

void ASPCharacterPlayer::Interaction(const FInputActionValue& Value)
{
	//Todo 멀티 연결하기
	SP_LOG(LogSPNetwork, Log, TEXT("Interaction"));

	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		//Todo if를 두번 해야하는건가?
		if (Actor->GetClass()->ImplementsInterface(USPGetInterface::StaticClass()))
		{
			ISPGetInterface* PotionActor = Cast<ISPGetInterface>(Actor);
			if (PotionActor)
			{
				SP_LOG(LogSPNetwork, Log, TEXT("IsPotion"));
				PotionActor->GetPotion();
				break;
			}
		}
	}
}

void ASPCharacterPlayer::ToggleMenuAction(const FInputActionValue& Value)
{
	HUDWidget->ToggleMenu();
}

void ASPCharacterPlayer::IceSKill(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("IceSKill"));
	if ((GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking || GetCharacterMovement()->MovementMode ==
		EMovementMode::MOVE_None) && !GetMesh()->GetAnimInstance()->Montage_IsPlaying(SkillMontage))
	{
		ServerRPCIceSkill(GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
	}
}

void ASPCharacterPlayer::ServerRPCIceSkill_Implementation(float AttackStartTime)
{
	if (bIsActiveIceSkill)
	{
		bIsActiveIceSkill = false;
		IceSkillComponent->ActivetedTimeStamp = GetWorld()->GetTime().GetRealTimeSeconds();
		AttackTimeDifference = GetWorld()->GetTimeSeconds() - AttackStartTime;
		AttackTimeDifference = FMath::Clamp(AttackTimeDifference, 0.0f, SlowAttackTime - 0.01f);

		FTimerHandle Handle;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
			                                       {
				                                       GetCharacterMovement()->SetMovementMode(
					                                       EMovementMode::MOVE_Walking);
			                                       }
		                                       ), SlowAttackTime - AttackTimeDifference, false, -1.0f);

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
	if ((GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking || GetCharacterMovement()->MovementMode ==
		EMovementMode::MOVE_None ) && !GetMesh()->GetAnimInstance()->Montage_IsPlaying(SkillIceMontage))
	{
		// bIsActiveSlowSkill = false;

		ServerRPCSlowSkill(GetWorld()->GetGameState()->GetServerWorldTimeSeconds());

		// if (!HasAuthority())
		// {
		// 	FTimerHandle Handle;
		// 	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		//
		// 	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
		// 		                                       {
		// 			                                       GetCharacterMovement()->SetMovementMode(
		// 				                                       EMovementMode::MOVE_Walking);
		// 		                                       }
		// 	                                       ), SlowAttackTime, false, -1.0f);
		//
		//
		// 	PlaySkillAnimation();
		// }
	}
}


void ASPCharacterPlayer::ServerRPCSlowSkill_Implementation(float AttackStartTime)
{
	if (bIsActiveSlowSkill)
	{
		bIsActiveSlowSkill = false;
		SlowSkillComponent->ActivetedTimeStamp = GetWorld()->GetTime().GetRealTimeSeconds();
		AttackTimeDifference = GetWorld()->GetTimeSeconds() - AttackStartTime;
		AttackTimeDifference = FMath::Clamp(AttackTimeDifference, 0.0f, SlowAttackTime - 0.01f);

		FTimerHandle Handle;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
			                                       {
				                                       GetCharacterMovement()->SetMovementMode(
					                                       EMovementMode::MOVE_Walking);
			                                       }
		                                       ), SlowAttackTime - AttackTimeDifference, false, -1.0f);

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
	CharacterToPlay->PlaySkillAnimation();
}

void ASPCharacterPlayer::OnRep_Potion()
{
	if (Potion)
	{
		SP_LOG(LogSPNetwork, Log, TEXT("%s"), TEXT("Potion YSE"));
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

void ASPCharacterPlayer::ServerRPCOrangePotionSpawn_Implementation()
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

void ASPCharacterPlayer::ServerRPCPurplePotionSpawn_Implementation()
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

void ASPCharacterPlayer::OnRep_PotionSpawn()
{
	SP_LOG(LogSPNetwork, Log, TEXT("%s"), TEXT("Potionspawn"));
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

void ASPCharacterPlayer::PlayThrowAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	UAnimInstance* TorsoAnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_JumpToSection(FName("End"), ThrowMontage);
}

void ASPCharacterPlayer::PlayStopAnimation()
{
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
			GetController()->GetControlRotation();
			FVector ForwardVector = UKismetMathLibrary::GetForwardVector(GetController()->GetControlRotation());
			float Mul = 1500.0f;
			Potion->Throw((ForwardVector + FVector{0.0f, 0.0f, 0.4f}) * Mul);
		}
		bIsThrowReady = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		bIsTurnReady = false;
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
	else
	{
		PlayStopAnimation();
		for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
		//플레이어 컨트롤러 목록을 서버에서 가지고 오기
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
						OtherPlayer->ClientRPCStopAnimation(this);
					}
				}
			}
		}
	}
}


// void ASPCharacterPlayer::ServerRPCSlowSkillMake_Implementation()
// {
//
// 	ASPSlowSkill* ProjectileSkill = NewObject<ASPSlowSkill>(this,ASPSlowSkill::StaticClass());
// 	// byclass
// 	if(ProjectileSkill)
// 	{
// 		UE_LOG(LogTemp,Log,TEXT("Make!!"));
//
// 		ProjectileSkill->SetActorLocation(SkillLocation->GetComponentLocation());
// 		ProjectileSkill->RegisterAllComponents();
// 		ProjectileSkill->SkillAction(this);
// 		
// 	}
// }

void ASPCharacterPlayer::ShowProjectilePath()
{
	if (IsLocallyControlled())
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
				//NewSplineMeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
				//NewSplineMeshComp->SetupAttachment(RootComponent);
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
}

void ASPCharacterPlayer::SetupHUDWidget(USPHUDWidget* InUserWidget)
{
	// USPTargetUI* TargetWidget = Cast<USPTargetUI>(InUserWidget);
	// if(TargetWidget)
	// {
	// 	UE_LOG(LogTemp, Log, TEXT("TEST"));
	// 	TargetWidget->UpdateTargetUI(bIsAiming);
	// 	this->OnAimChanged.AddUObject(TargetWidget, &USPTargetUI::UpdateTargetUI);
	// }
	SlowSkillComponent->OnSlowCDChange.AddUObject(InUserWidget, &USPHUDWidget::UpdateSlowCDTime);
	IceSkillComponent->OnIceCDChange.AddUObject(InUserWidget, &USPHUDWidget::UpdateIceCDTime);
}

void ASPCharacterPlayer::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector TraceStart{GetPawnViewLocation()};
	FVector TraceEnd{TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance)};

	float LookDirection = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector());

	if (LookDirection > 0) //양수 음수에 따라 같은 방향인지 아닌지 판단
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);


		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this); //나는 쏘는 사람이니까 나를 무시해야 함
		FHitResult TraceHit;

		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(USPInteractionInterface::StaticClass()))
			{
				if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
				{
					FoundInteractable(TraceHit.GetActor());
					return;
				}

				if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
				{
					return;
				}
			}
		}
	}
	NoInteractableFound();
}

void ASPCharacterPlayer::FoundInteractable(AActor* NewInteractable)
{
	//이전 상호 작용이 있는지 확인
	if (IsInteracting())
	{
		EndInteract();
	}

	if (InteractionData.CurrentInteractable) //상호작용 데이터 있으면!
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	//Todo 좀 더 효율적이게 바꾸기
	if (HUDWidget)
	{
		HUDWidget->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}

	// TargetInteractable->BeginFocus();
}

void ASPCharacterPlayer::NoInteractableFound()
{
	if (IsInteracting())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	}

	//pickup 후 물체가 세계에서 사라짐
	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject())) //한번 더 확인
		{
			TargetInteractable->EndFocus();
		}

		if (HUDWidget)
		{
			HUDWidget->HideInteractionWidget();
		}
		// 인터렉션 위젯 지우기
		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void ASPCharacterPlayer::BeginInteract()
{
	PerformInteractionCheck(); //인터렉트 대상이 변하지 않는지 체크

	//인터렉트 가능한 대상 있는지
	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject())) //한번 더 확인
		{
			TargetInteractable->BeginInteract();

			//시간을 확인한다!
			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f)) //상호작용 시간 비교
			{
				// 상호작용 시간이 거의 즉시이므로 바로 Interact 함수 호출
				Interact();
			}
			else
			{
				// 상호작용에 시간이 필요하므로 타이머를 설정하여 지정된 시간 후에 Interact 함수 호출
				GetWorldTimerManager().SetTimer(TimerHandle_Interaction,
				                                this,
				                                &ASPCharacterPlayer::Interact,
				                                TargetInteractable->InteractableData.InteractionDuration,
				                                false);
			}
		}
	}
}

void ASPCharacterPlayer::EndInteract()
{
	//상호작용을 제대로 종료하면 지운다. 타이머 끝났다고 가정하고 지운다.
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}
}

void ASPCharacterPlayer::Interact()
{
	//타이머 끝났다고 가정하고 지운다.

	ServerRPCInteract();
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	// if(IsValid(TargetInteractable.GetObject()))
	// {
	// 	TargetInteractable->Interact(this);
	// }
	// USPTargetUI* TargetWidget = Cast<USPTargetUI>(InUserWidget);
	// Target->GetWidget().UpdateTarget;
	// if(TargetWidget)
	// {
	// 	UE_LOG(LogTemp, Log, TEXT("SetupTargetWidget"));
	// 	TargetWidget->UpdateTargetUI(bIsAiming);
	// 	//this->OnAimChanged.AddUObject(TargetWidget, &USPTargetUI::UpdateTargetUI);
	// }
}

void ASPCharacterPlayer::ServerRPCInteract_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact(this);
	}
}

void ASPCharacterPlayer::UpdateInteractionWidget() const
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		HUDWidget->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}
}

void ASPCharacterPlayer::DropItem(USPItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	if (PlayerInventory->FindMatchingItem(ItemToDrop))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		const FVector SpawnLocation{GetActorLocation() + (GetActorForwardVector() * 50.f)};

		const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

		const int32 RemovedQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);

		ASPPickup* Pickup = GetWorld()->SpawnActor<ASPPickup>(ASPPickup::StaticClass(), SpawnTransform, SpawnParams);

		Pickup->InitializeDrop(ItemToDrop, RemovedQuantity);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item to drop was somehow null"));
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
	DOREPLIFETIME(ASPCharacterPlayer, bIsTurnReady);
	DOREPLIFETIME(ASPCharacterPlayer, bIsThrowReady);
	DOREPLIFETIME(ASPCharacterPlayer, bIsHolding);
	// DOREPLIFETIME(ASPCharacterPlayer, bIsActiveSlowSkill);
	//bIsActiveSlowSkill
	// DOREPLIFETIME(ASPCharacterPlayer, bIsActiveSlowSkill);
	// DOREPLIFETIME(ASPCharacterPlayer, SkillCastComponent);
	//SkillCastComponent
}

void ASPCharacterPlayer::PlaySkillAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(SkillMontage);
}

void ASPCharacterPlayer::PlayIceSkillAnimation()
{
	UE_LOG(LogTemp, Log, TEXT("PlayIceSkillAnimation"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(SkillIceMontage);
}

void ASPCharacterPlayer::HitSlowSkillResult()
{
	// Cast<USPCharacterMovementComponent>(GetMovementComponent());
	// NetTESTRPCSlowSkill();
	bIsDamage = true;
	GetCharacterMovement()->MaxWalkSpeed = 100.f;
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
		                                       {
			                                       GetCharacterMovement()->MaxWalkSpeed = 500.f;
			                                       bIsDamage = false;
		                                       }
	                                       ), 5, false, -1.0f);
}

void ASPCharacterPlayer::HitIceSkillResult()
{
	bIsDamage = true;
	// GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
		                                       {
			                                       GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			                                       bIsDamage = false;
		                                       }
	                                       ), 5, false, -1.0f);
}
void ASPCharacterPlayer::NetTESTRPCSlowSkill_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 100.f;
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
		                                       {
			                                       GetCharacterMovement()->MaxWalkSpeed = 500.f;
		                                       }
	                                       ), 5, false, -1.0f);
	// USPCharacterMovementComponent* SPMovement = Cast<USPCharacterMovementComponent>(GetCharacterMovement());
	// if(SPMovement)
	// {
	// 	// SPMovement->SetSlowSkillCommand();
	// }
}


void ASPCharacterPlayer::ServerRPCBlackPotionSpawn_Implementation()
{
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
}

void ASPCharacterPlayer::ServerRPCGraping_Implementation()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	if (false == bIsHolding)
	{
		SP_LOG(LogSPNetwork, Log, TEXT("%s"), TEXT("ServerRPCGraping_Implementation!!"));

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
			if (HitSuccess && outHitResult.Component->Mobility == EComponentMobility::Movable)
			{
				outHitResult.Component->SetSimulatePhysics(true);
				HitComponent = outHitResult.GetComponent();

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

void ASPCharacterPlayer::ServerRPCStopGraping_Implementation()
{
	if (bIsHolding && HitComponent->IsSimulatingPhysics())
	{
		bIsHolding = false;
		PhysicsHandleComponent->ReleaseComponent();
		HitComponent->AddImpulse(FollowCamera->GetForwardVector() * HitDistance, NAME_None, true);
		HitComponent = nullptr;
	}
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
	if (false == bIsDamage)
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
	}
}

void ASPCharacterPlayer::ServerRPCSpeedUp_Implementation()
{
	if (false == bIsDamage)
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
