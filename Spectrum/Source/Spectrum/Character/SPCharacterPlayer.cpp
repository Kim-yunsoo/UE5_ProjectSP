// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SPCharacterPlayer.h"
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
//#include "UI/SPHUDWidget.h"

ASPCharacterPlayer::ASPCharacterPlayer()
{
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
		GravityArrow->SetupAttachment(GetMesh());
		GravityArrow->SetRelativeRotation(FRotator(0, 0, 0));
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_Jump.IA_SP_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_ChangeControl.IA_SP_ChangeControl'"));
	if (nullptr != InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_ShoulderMove.IA_SP_ShoulderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_ShoulderLook.IA_SP_ShoulderLook'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_QuaterMove.IA_SP_QuaterMove'"));
	if (nullptr != InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> SpeedUpActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_SpeedUp.IA_SP_SpeedUp'"));
	if (nullptr != SpeedUpActionRef.Object)
	{
		SpeedUpAction = SpeedUpActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> AimActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_MouseRight.IA_SP_MouseRight'"));
	if (nullptr != AimActionRef.Object)
	{
		MouseRight = AimActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MouseLeftRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_MouseLeft.IA_SP_MouseLeft'"));
	if (nullptr != MouseLeftRef.Object)
	{
		MouseLeft = MouseLeftRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> BlackFourRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_BlackPotionSpawn.IA_SP_BlackPotionSpawn'"));
	if (nullptr != BlackFourRef.Object)
	{
		BlackFour = BlackFourRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ThrowCtrlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_Throw.IA_SP_Throw'"));
	if (nullptr != ThrowCtrlRef.Object)
	{
		ThrowCtrl = ThrowCtrlRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> GreenOneRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Spectrum/Input/Actions/IA_SP_GreenPotionSpawn.IA_SP_GreenPotionSpawn'"));
	if (nullptr != GreenOneRef.Object)
	{
		GreenOne = GreenOneRef.Object;
	}
	

	/*static ConstructorHelpers::FObjectFinder<UAnimMontage> ThrowMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Spectrum/Animation/AniMeta/Man/AM_SP_Throw.AM_SP_Throw'"));
	if (ThrowMontageRef.Object)
	{
		ThrowMontage = ThrowMontageRef.Object;
	}*/

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshforSplineRef(TEXT("/Script/Engine.StaticMesh'/Game/Spectrum/SM_MERGED_StaticMeshActor_90.SM_MERGED_StaticMeshActor_90'"));
	if (StaticMeshforSplineRef.Object)
	{
		StaticMeshforSpline = StaticMeshforSplineRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> DecalMaterialRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Box/MagicCircle/Material/MI_MagicCircleDecal_16.MI_MagicCircleDecal_16'"));
	if (DecalMaterialRef.Object)
	{
		DecalMaterial = DecalMaterialRef.Object;
	}

	CurrentCharacterControlType = ECharacterControlType::Shoulder;
	LastInput = FVector2D::ZeroVector;
	//bIsAiming = false;
	//bIsHolding = false;
	HitComponent = nullptr;
	bIsSpawn = false;
	bIsThrowReady = false;
	bIsTurnReady = false;
	bIsTurnLeft = false;
	bIsTurnReady = false;
	HitDistance = 1200.f;
}

void ASPCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	SetCharacterControl(CurrentCharacterControlType);
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &ASPCharacterPlayer::HandleMontageAnimNotify);
}

void ASPCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	bool ForceSendPacket = false;

	if (LastDesiredInput != DesiredInput)
	{
		ForceSendPacket = true;
		LastDesiredInput = DesiredInput;
	}

	//if (DesiredInput == FVector2D::Zero()) {
	//	SetMoveState(Protocol::MOVE_STATE_IDLE);
	//	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("IDLE")));
	//}
	//else {
	//	SetMoveState(Protocol::MOVE_STATE_RUN);
	//	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("RUN")));
	//}


	FVector Velocity = GetVelocity();
	if (!Velocity.IsNearlyZero())
		SetMoveState(Protocol::MOVE_STATE_RUN);
	else
		SetMoveState(Protocol::MOVE_STATE_IDLE);
	
		// 0.1
	MovePacketSendTimer -= DeltaTime*10;

	if (MovePacketSendTimer <= 0 || ForceSendPacket)
	{
		MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

		Protocol::C_MOVE MovePkt;

		{
			Protocol::PositionInfo* Info = MovePkt.mutable_info();
			Info->CopyFrom(*PlayerInfo);
			Info->set_yaw(DesiredYaw);
			Info->set_state(GetMoveState());
			Info->set_is_aiming(bIsAiming);
			Info->set_is_holding(bIsHolding);
			Info->set_is_jumping(bIsJumping);

			////Info â�� ���
			//if(GetMoveState()== Protocol::MOVE_STATE_IDLE)
			//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("IDLE")));
			//else if(GetMoveState() == Protocol::MOVE_STATE_RUN)
			//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("RUN")));

			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("%f %f %f"),
			//	GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("%f %f %f"),
			//	PlayerInfo->x(), PlayerInfo->y(), PlayerInfo->z()));
		}

		SEND_PACKET(MovePkt);
	}

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
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASPCharacterPlayer::StopJumping);

		EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &ASPCharacterPlayer::ChangeCharacterControl);

		EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &ASPCharacterPlayer::ShoulderMove);

		EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &ASPCharacterPlayer::ShoulderLook);
		//EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::None, this, &ASPCharacterPlayer::StopShoulderLook);

		EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &ASPCharacterPlayer::QuaterMove);
		EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Completed, this, &ASPCharacterPlayer::QuaterMove);

		EnhancedInputComponent->BindAction(SpeedUpAction, ETriggerEvent::Triggered, this, &ASPCharacterPlayer::SpeedUp);
		EnhancedInputComponent->BindAction(SpeedUpAction, ETriggerEvent::Completed, this, &ASPCharacterPlayer::StopSpeedUp);

		EnhancedInputComponent->BindAction(MouseLeft, ETriggerEvent::Triggered, this, &ASPCharacterPlayer::Graping);
		EnhancedInputComponent->BindAction(MouseLeft, ETriggerEvent::Completed, this, &ASPCharacterPlayer::StopGraping);

		EnhancedInputComponent->BindAction(MouseRight, ETriggerEvent::Triggered, this, &ASPCharacterPlayer::Aiming);
		EnhancedInputComponent->BindAction(MouseRight, ETriggerEvent::Completed, this, &ASPCharacterPlayer::StopAiming);

		EnhancedInputComponent->BindAction(BlackFour, ETriggerEvent::Triggered, this, &ASPCharacterPlayer::BlackPotionSpawn);

		EnhancedInputComponent->BindAction(ThrowCtrl, ETriggerEvent::Triggered, this, &ASPCharacterPlayer::AimPotion);
		EnhancedInputComponent->BindAction(ThrowCtrl, ETriggerEvent::Completed, this, &ASPCharacterPlayer::ThrowPotion);

		EnhancedInputComponent->BindAction(GreenOne, ETriggerEvent::Triggered, this, &ASPCharacterPlayer::GreenPotionSpawn);

	}
}

void ASPCharacterPlayer::SetCharacterControlData(const USPCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

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
	FVector TargetRelativeLocation{ 0,0,0 };
	FRotator TargetRelativeRotation{ 0,0,0 };
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(FollowCamera, TargetRelativeLocation, TargetRelativeRotation, true, true, 0.5, true, EMoveComponentAction::Type::Move, LatentInfo);
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
	USPCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
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

		if (GetControlRotation().Yaw > PreControlYawRotation)
		{
			//UE_LOG(LogTemp, Log, TEXT("TEST1"));
			bIsTurnRight = true;
			bIsTurnLeft = false;
		}
		else
		{
				bIsTurnRight = false;
				bIsTurnLeft = true;
		}
		PreControlYawRotation = GetControlRotation().Yaw;
	}
}

void ASPCharacterPlayer::StopShoulderLook(const FInputActionValue& Value)
{
		bIsTurnRight = false;
		bIsTurnLeft = false;
}

void ASPCharacterPlayer::SpeedUp(const FInputActionValue& Value)
{
	if (false == bIsAiming && false == bIsHolding)
	{
		GetCharacterMovement()->MaxWalkSpeed = 900.f;
	}
}

void ASPCharacterPlayer::StopSpeedUp(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
}

void ASPCharacterPlayer::Aiming(const FInputActionValue& Value)
{
	if (false == bIsHolding) {
		bIsAiming = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
		FollowCamera->AttachToComponent(SpringArm, AttachmentRules, NAME_None);
		CameraMove();
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
		FollowCamera->AttachToComponent(CameraBoom, AttachmentRules, NAME_None);
		CameraMove();
	}
}

void ASPCharacterPlayer::StopAiming(const FInputActionValue& Value)
{

	bIsAiming = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	FollowCamera->K2_AttachToComponent(CameraBoom, NAME_None, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
	FollowCamera->AttachToComponent(CameraBoom, AttachmentRules, NAME_None);
	CameraMove();
}

void ASPCharacterPlayer::Graping(const FInputActionValue& Value)
{
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
			bool TransSuccess = PlayerController->DeprojectScreenPositionToWorld(0.5, 0.5, WorldLocation, WorldDirection);

			FVector SphereLocationEnd = ReseltFoward * 10000000 + SphereLocationStart;

			TArray<TEnumAsByte<EObjectTypeQuery>> EmptyObjectTypes;
			EDrawDebugTrace::Type drawDebugType = EDrawDebugTrace::ForDuration;
			TArray<AActor*> actorsToIgnore;
			FLinearColor RedColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
			FLinearColor GreenColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);
			Params.bTraceComplex = true;
			float DrawTime = 5.0f;

			bool HitSuccess = GetWorld()->LineTraceSingleByChannel(outHitResult, SphereLocationStart, SphereLocationEnd, ECC_GameTraceChannel1, Params);
			if (HitSuccess && outHitResult.Component->Mobility == EComponentMobility::Movable)
			{
				outHitResult.Component->SetSimulatePhysics(true);
				HitComponent = outHitResult.GetComponent();

				// ������ ���� ����
				AActor* OwnerActor = HitComponent->GetOwner();
				ASPObject* MyActor = static_cast<ASPObject*>(OwnerActor);
				if (MyActor)
				{
					// ĳ���� ����, MyActor�� ����� ���� ����
					MyActor->ObjectInfo->set_is_holding(true);
					MyActor->ObjectInfo->set_x(MyActor->K2_GetActorLocation().X);
					MyActor->ObjectInfo->set_y(MyActor->K2_GetActorLocation().Y);
					MyActor->ObjectInfo->set_z(MyActor->K2_GetActorLocation().Z);
					

					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("%lld"), 
						MyActor->ObjectInfo->object_id()));

				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("cast fail")));
				}

				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("%f %f %f"),
				//	GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z));

				// UE_LOG ��ũ�θ� ����Ͽ� �α׸� ����մϴ�.
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

				bool Success = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), SphereTracePoint, SphereTracePoint, Radius, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHits, true, GreenColor1, RedColor1, DrawTime1);

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
					FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
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
			// ���� ���� �� --> ���⼭ HitComponent��  is_holding ����
			AActor* OwnerActor = HitComponent->GetOwner();
			ASPObject* MyActor = Cast<ASPObject>(OwnerActor);
			MyActor->ObjectInfo->set_is_holding(false);

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

		AActor* OwnerActor = HitComponent->GetOwner();
		ASPObject* MyActor = Cast<ASPObject>(OwnerActor);
		MyActor->ObjectInfo->set_is_holding(false);
	}
}

void ASPCharacterPlayer::AimPotion(const FInputActionValue& Value)
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

void ASPCharacterPlayer::ThrowPotion(const FInputActionValue& Value)
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

void ASPCharacterPlayer::Jumping(const FInputActionValue& Value)
{
	if (!bIsAiming)
	{
		bPressedJump = true;
		JumpKeyHoldTime = 0.0f;
	}
	// ���� ������Ʈ ����
	//SetMoveState(Protocol::MOVE_STATE_JUMP);
	SetJumping();
}

void ASPCharacterPlayer::StopJumping(const FInputActionValue& Value)
{
	bPressedJump = false;
	ResetJumpState();
	/*bIsJumping = false;*/

	// ���� ������Ʈ ����
	//SetMoveState(Protocol::MOVE_STATE_IDLE);
	ResetJumping();
}

void ASPCharacterPlayer::BlackPotionSpawn(const FInputActionValue& Value)
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

void ASPCharacterPlayer::GreenPotionSpawn(const FInputActionValue& Value)
{
	if (false == bIsSpawn)
	{
		FVector ItemLocation = GetMesh()->GetSocketLocation("Item_Socket");
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
		Potion = GetWorld()->SpawnActor<ASPGreenPotion>(ASPGreenPotion::StaticClass(), GetMesh()->GetSocketLocation("Item_Socket"), FRotator{ 0.0f, 0.0f, 0.0f }, SpawnParams);
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

void ASPCharacterPlayer::HandleMontageAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
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
			+ FVector{ 0.0f,0.0f,0.4f }) * 1500.0f;
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
			OutLastTraceDestination, StartPos, LaunchVelocity, true, ProjectileRadius, TraceChannel, false, ActorsToIgnore,
			DrawDebugType, DrawDebugTime, SimFrequency, MaxSimTime, OverrideGravityZ);

		FHitResult SweepHitResult;
		/*ProjectileCircle->SetWorldLocation(OutHit.Location, false, &SweepHitResult, ETeleportType::None);
		ProjectileCircle->SetVisibility(true, false);*/
		FVector DecalSize{ 100,200,200 };
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalMaterial, DecalSize, OutHit.Location, GetControlRotation(), 0.1);



		for (int i = 0; i < OutPathPositions.Num(); i++)
		{
			Projectile_Path->AddSplinePointAtIndex(OutPathPositions[i], i, ESplineCoordinateSpace::Type::Local, true);
		}
		for (int i = 0; i < Projectile_Path->GetNumberOfSplinePoints() - 1; ++i)
		{
			UClass* whyClass = USplineMeshComponent::StaticClass();
			FTransform RelativeTransform = FTransform();

			USplineMeshComponent* NewSplineMeshComp = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
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
				NewSplineMeshComp->SetStartAndEnd(StartPointLocation, StartPointTangent, EndPointLocation, EndPointTangent, true);
			}
			SplineCompArray.Emplace(NewSplineMeshComp);
			NewSplineMeshComp->RegisterComponent();


		}
		FTimerHandle TimerHandle;
		float DelayTime = 0.01f;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() {
			ShowProjectilePath();
			}, DelayTime, false);
	}
	else
	{
		//ProjectileCircle->SetVisibility(false);
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

