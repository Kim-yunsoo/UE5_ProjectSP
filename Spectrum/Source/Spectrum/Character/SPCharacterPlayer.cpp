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
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"


ASPCharacterPlayer::ASPCharacterPlayer()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 150.f;
	SpringArm->SetRelativeLocation(FVector(0.0, 0.0, 50.0));
	SpringArm->SocketOffset.Set(0.0, 30, 10);
	SpringArm->bUsePawnControlRotation = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->SetRelativeLocation(FVector(0.0, 0.0, 63.990518));
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraBoom->SetRelativeLocation(FVector(0.0, 29.020852, 11.28551));
	FollowCamera->bUsePawnControlRotation = false;

	PotionThrowStartLocation = CreateDefaultSubobject<USceneComponent>(TEXT("PotionThrowStartLocation"));
	PotionThrowStartLocation->SetupAttachment(RootComponent);
	PotionThrowStartLocation->SetRelativeLocation(FVector(0.0, -48.566387, 67.547544));

	Projectile_Path = CreateDefaultSubobject<USplineComponent>(TEXT("Projectile_Path"));
	Projectile_Path->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CircleRef(TEXT("/Script/Engine.StaticMesh'/Game/Spectrum/Prop/SM_Circle.SM_Circle'"));
	if (CircleRef.Object)
	{
		ProjectileCircle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileCircle"));
		ProjectileCircle->SetStaticMesh(CircleRef.Object);
		ProjectileCircle->SetupAttachment(RootComponent);
		ProjectileCircle->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
		ProjectileCircle->SetVisibility(false);
	}




	GravityArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("GravityArrow"));
	if (GravityArrow)
	{
		GravityArrow->SetupAttachment(FollowCamera);
		GravityArrow->SetRelativeLocation(FVector(811.303858, 62.924746, 64.091908));
		GravityArrow->SetRelativeRotation(FRotator(0, 0, 0));
	}

	// 카메라 시점에 따른 에셋 로드
	// Input
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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ThrowMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Spectrum/Animation/AniMeta/Man/AM_SP_Throw.AM_SP_Throw'"));
	if (ThrowMontageRef.Object)
	{
		ThrowMontage = ThrowMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SplineCoinRef(TEXT("/Script/Engine.StaticMesh'/Game/Spectrum/Prop/SM_TEST.SM_TEST'"));
	if (SplineCoinRef.Object)
	{
		StaticMeshforSpline = SplineCoinRef.Object;
	}

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> SplineCoinRef(TEXT("/Script/Engine.StaticMesh'/Game/Spectrum/Prop/SM_Projectile.SM_Projectile'"));
	//SplineCoin->SetStaticMesh(SplineCoinRef.Object);

	CurrentCharacterControlType = ECharacterControlType::Shoulder;
	LastInput = FVector2D::ZeroVector;
	//bIsAiming = false;
	//bIsHolding = false;
	HitComponent = nullptr;
	bIsSpawn = false;
	bIsThrowReady = false;
	HitDistance = 1200.f;
}

void ASPCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	SetCharacterControl(CurrentCharacterControlType);
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &ASPCharacterPlayer::HandleMontageAnimNotify);
	//Add Input Mapping Context
	//if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	//{
	//	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	//	{
	//		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	//	}
	//}
}

void ASPCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 보내는게 맞는지 판정
	bool ForceSendPacket = false;

	if (LastDesiredInput != DesiredInput)
	{
		ForceSendPacket = true;
		LastDesiredInput = DesiredInput;
	}

	// 보낼 상태 정보(움직이고 있는지, 아닌지)
	if (DesiredInput == FVector2D::Zero()) {
		SetMoveState(Protocol::MOVE_STATE_IDLE);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("IDLE")));
	}
	else {
		SetMoveState(Protocol::MOVE_STATE_RUN);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("RUN")));
	}
	
		// 0.1초마다 서버로 이동 패킷을 전송
	MovePacketSendTimer -= DeltaTime * 10;

	if (MovePacketSendTimer <= 0 || ForceSendPacket)
	{
		MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

		Protocol::C_MOVE MovePkt;

		// 현재 상태 정보를 패킷에 담아서 서버로 전송
		{
			Protocol::PositionInfo* Info = MovePkt.mutable_info();
			Info->CopyFrom(*PlayerInfo);
			Info->set_yaw(DesiredYaw);
			Info->set_state(GetMoveState());
			Info->set_is_aiming(bIsAiming);
			Info->set_is_holding(bIsHolding);
			Info->set_is_jumping(bIsJumping);

			////Info 창에 출력
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

	// 중력총 클라이언트 코드 
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
		EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Completed, this, &ASPCharacterPlayer::ShoulderMove);

		EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &ASPCharacterPlayer::ShoulderLook);

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

		// 서버로 이동 패킷을 전송하기 위해 최종 결과물 저장
		{
			DesiredInput = MovementVector;

			// 최종적으로 바라보는 방향을 계산
			DesiredMoveDirection = FVector::ZeroVector;
			DesiredMoveDirection -= ForwardDirection * MovementVector.Y;
			DesiredMoveDirection += RightDirection * MovementVector.X;
			DesiredMoveDirection.Normalize();

			const FVector Location = GetActorLocation();
			FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(Location, Location + DesiredMoveDirection);
			DesiredYaw = Rotator.Yaw;
		}

	}
}

void ASPCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASPCharacterPlayer::SpeedUp(const FInputActionValue& Value)
{
	if (false == bIsAiming && false == bIsHolding) {
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
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
		FollowCamera->AttachToComponent(SpringArm, AttachmentRules, NAME_None);
		CameraMove();

	}
	else // bIsHolding == true인 경우
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
		FollowCamera->AttachToComponent(CameraBoom, AttachmentRules, NAME_None);
		CameraMove();
	}
}

void ASPCharacterPlayer::StopAiming(const FInputActionValue& Value)
{
	bIsAiming = false;
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
				outHitResult.Component->SetSimulatePhysics(true); //시뮬레이션 켜기 
				HitComponent = outHitResult.GetComponent();

				// 물건의 정보 수정
				AActor* OwnerActor = HitComponent->GetOwner();
				ASPObject* MyActor = static_cast<ASPObject*>(OwnerActor);
				if (MyActor)
				{
					// 캐스팅 성공, MyActor를 사용해 로직 수행
					MyActor->ObjectInfo->set_is_holding(true);
					MyActor->ObjectInfo->set_x(MyActor->K2_GetActorLocation().X);
					MyActor->ObjectInfo->set_y(MyActor->K2_GetActorLocation().Y);
					MyActor->ObjectInfo->set_z(MyActor->K2_GetActorLocation().Z);
				}
				else
				{
					// 캐스팅 실패, 에러 로깅 또는 대체 로직 수행
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("cast fail")));
				}

				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("%f %f %f"),
				//	GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z));

				// UE_LOG 매크로를 사용하여 로그를 출력합니다.

				if (HitComponent && HitComponent->IsSimulatingPhysics())
				{
					PhysicsHandleComponent->GrabComponentAtLocation(
						HitComponent,      // 잡을 컴포넌트
						NAME_None,         // 본 이름 (이 경우 빈 값)
						HitComponent->K2_GetComponentLocation()
					);

					bIsHolding = true;
					FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
					FollowCamera->AttachToComponent(CameraBoom, AttachmentRules, NAME_None);
					CameraMove();
				}
			}

			const FColor LineColor = HitSuccess ? FColor::Green : FColor::Red;

			// 라인 트레이스 경로 디버그 라인 그리기
			DrawDebugLine(
				GetWorld(),
				SphereLocationStart,
				SphereLocationEnd,
				LineColor,
				false, // 지속 시간 동안 존재하지 않음
				5.0f, // 5초 동안 표시
				0, // 뎁스 우선순위
				1.0f // 라인 굵기
			);

			// 충돌이 발생했다면, 충돌 지점에 디버그 포인트 그리기
			if (HitSuccess)
			{
				DrawDebugPoint(
					GetWorld(),
					outHitResult.ImpactPoint, // 충돌 지점
					10.0f, // 포인트 크기
					FColor::Blue, // 포인트 색상
					false, // 지속 시간 동안 존재하지 않음
					5.0f // 5초 동안 표시
				);
			}
		}
	}
	else // bIsHolding == true인 경우 
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

		// 물건 놨을 때 --> 여기서 HitComponent의  is_holding 끄기
		//AActor* OwnerActor = HitComponent->GetOwner();
		//ASPObject* MyActor = Cast<ASPObject>(OwnerActor);
		//MyActor->ObjectInfo->set_is_holding(false);
	}
}

void ASPCharacterPlayer::AimPotion(const FInputActionValue& Value)
{
	if (bIsSpawn)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(ThrowMontage, 1.0f);
	}
}

void ASPCharacterPlayer::ThrowPotion(const FInputActionValue& Value)
{
	if (bIsThrowReady)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_JumpToSection(FName("End"), ThrowMontage);
		bIsThrowReady = false;
		if (BlackPotion)
		{
			GetController()->GetControlRotation();
			FVector ForwardVector = UKismetMathLibrary::GetForwardVector(GetController()->GetControlRotation());
			float Mul = 1500.0f;
			BlackPotion->Throw((ForwardVector + FVector{ 0.0f,0.0f,0.4f }) * Mul);
		}
		bIsSpawn = false;
		BlackPotion = nullptr;
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
	// 무브 스테이트 설정
	SetMoveState(Protocol::MOVE_STATE_JUMP);
	SetJumping();
}

void ASPCharacterPlayer::StopJumping(const FInputActionValue& Value)
{
	bPressedJump = false;
	ResetJumpState();
	/*bIsJumping = false;*/

	// 무브 스테이트 설정
	//SetMoveState(Protocol::MOVE_STATE_IDLE);
	ResetJumping();
}

void ASPCharacterPlayer::BlackPotionSpawn(const FInputActionValue& Value)
{
	if (false == bIsSpawn)
	{
		FVector ItemLocation = GetMesh()->GetSocketLocation("Item_Socket");
		// 액터 타입 캐스팅 
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
		BlackPotion = GetWorld()->SpawnActor<ASPBlackPotion>(ASPBlackPotion::StaticClass(), GetMesh()->GetSocketLocation("Item_Socket"), FRotator{ 0.0f, 0.0f, 0.0f }, SpawnParams);
		bIsSpawn = true;
		if (BlackPotion)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
			BlackPotion->AttachToComponent(this->GetMesh(), AttachmentRules, FName{ "Item_Socket" });
		}
	}
	else // bIsSpawn == true인 경우
	{
		if (BlackPotion)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
			BlackPotion->AttachToComponent(this->GetMesh(), AttachmentRules, FName{ "Item_Socket" });
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

		// 파라미터 설정
		FHitResult OutHit;
		TArray<FVector> OutPathPositions;
		FVector OutLastTraceDestination;

		FVector StartPos = PotionThrowStartLocation->GetComponentLocation();
		//GetController()->GetControlRotation();
		//FVector LaunchVelocity = ; // 발사 속도
		FVector LaunchVelocity = (UKismetMathLibrary::GetForwardVector(GetController()->GetControlRotation())
			+ FVector{ 0.0f,0.0f,0.4f }) * 1500.0f;
		//(ForwardVector + FVector{ 0.0f,0.0f,0.4f })* Mul
		float ProjectileRadius = 0.0f;
		TEnumAsByte<ECollisionChannel> TraceChannel = ECollisionChannel::ECC_Camera; // 충돌 채널
		TArray<AActor*> ActorsToIgnore; // 무시할 액터들
		AActor* SelfActor = GetOwner();
		ActorsToIgnore.Add(SelfActor);
		EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None; // 디버그 정보 타입
		float DrawDebugTime = 0.0f; // 디버그 정보 표시 시간
		float SimFrequency = 15.0f; // 시뮬레이션 주파수
		float MaxSimTime = 2.0f; // 최대 시뮬레이션 시간
		float OverrideGravityZ = 0.0; // 중력 값

		UGameplayStatics::Blueprint_PredictProjectilePath_ByTraceChannel(GetWorld(), OutHit, OutPathPositions,
			OutLastTraceDestination, StartPos, LaunchVelocity, true, ProjectileRadius, TraceChannel, false, ActorsToIgnore,
			DrawDebugType, DrawDebugTime, SimFrequency, MaxSimTime, OverrideGravityZ);

		FHitResult SweepHitResult;
		ProjectileCircle->SetWorldLocation(OutHit.Location, false, &SweepHitResult, ETeleportType::None);
		ProjectileCircle->SetVisibility(true, false);

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
			NewSplineMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			NewSplineMeshComp->SetCollisionProfileName(TEXT("SplineCollision")); 
			NewSplineMeshComp->SetGenerateOverlapEvents(true);
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

				//NewSplineMeshComp->SetWorldLocation(StartPointLocation);

				FColor BeautyfulColor = FColor(
					FMath::RandRange(30, 200),
					FMath::RandRange(30, 200),
					FMath::RandRange(30, 200),
					1.f);
				/*FVector StartPointLocation, EndPointLocation;*/
			/*	DrawDebugLine(
					GetWorld(),
					StartPointLocation,
					EndPointLocation,
					BeautyfulColor,
					false,
					5.0f, 0, 5);*/
			}
			FVector Location = NewSplineMeshComp->K2_GetComponentLocation();
			//DrawDebugSphere(GetWorld(), Location, Radius, 20, Color3, false, 5.0f);
			SplineCompArray.Emplace(NewSplineMeshComp);
			NewSplineMeshComp->RegisterComponent();

			//UActorComponent* ActorComponent= AddComponentByClass(USplineMeshComponent::StaticClass(),true, RelativeTransform,false);
			//if (/*USplineMeshComponent* SplineMeshComponent = Cast<USplineMeshComponent>(ActorComponent)*/)
			//{
			//	SplineMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			//	SplineMeshComponent->RegisterComponent();
			//}
			// 
			// 
			// 
				//RootComponent->AttachToComponent(ActorComponent, FAttachmentTransformRules::KeepRelativeTransform);
	/*		FName TemplateName = FName("SM_Projectile");
			UActorComponent* ActorComponent= this->AddComponent(TemplateName,true,  FTransform(), nullptr, false);
			FVector SplineLocation= Projectile_Path->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
			FVector SplineTangent= Projectile_Path->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
			Cast<USplineMeshComponent>(ActorComponent)->SetStartAndEnd(SplineLocation, SplineTangent, SplineLocation, SplineTangent, true);
			SplineMeshComponents.Add(Cast<USplineMeshComponent>(ActorComponent));*/
		}
		FTimerHandle TimerHandle;
		float DelayTime = 0.05f;

		// FTimerManager를 이용하여 Delay
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() {
			ShowProjectilePath();
			}, DelayTime, false);
	}
	else
	{
		ProjectileCircle->SetVisibility(false);
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


	// 서버로 이동 패킷을 전송하기 위해 최종 결과물 저장
	{
		DesiredInput = MovementVector;

		// 최종적으로 바라보는 방향을 계산
		DesiredMoveDirection = FVector::ZeroVector;
		DesiredMoveDirection += ForwardDirection * MovementVector.Y;
		DesiredMoveDirection += RightDirection * MovementVector.X;
		DesiredMoveDirection.Normalize();

		const FVector Location = GetActorLocation();
		FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(Location, Location + DesiredMoveDirection);
		DesiredYaw = Rotator.Yaw;
	}
}

