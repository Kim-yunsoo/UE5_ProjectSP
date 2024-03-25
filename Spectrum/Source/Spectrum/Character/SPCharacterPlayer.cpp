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
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"





ASPCharacterPlayer::ASPCharacterPlayer()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->SetRelativeLocation(FVector(0.0, 0.0, 63.990518));

	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraBoom->SetRelativeLocation(FVector(0.0, 29.020852, 11.28551));
	FollowCamera->bUsePawnControlRotation = false;

	GravityArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("GravityArrow"));
	if (GravityArrow)
	{
		GravityArrow->SetupAttachment(FollowCamera);
		GravityArrow->SetRelativeLocation(FVector(811.303858, 13.162654, 0.0f));
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

	CurrentCharacterControlType = ECharacterControlType::Shoulder;

	LastInput = FVector2D::ZeroVector;

	bIsAiming = false;
	bIsHolding = false;
	HitComponent = nullptr;
	HitDistance = 1200.f;


}

void ASPCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterControl(CurrentCharacterControlType);

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}


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
	/*if (DesiredInput.SizeSquared() <= KINDA_SMALL_NUMBER)
		SetMoveState(Protocol::MOVE_STATE_IDLE);
	else
		SetMoveState(Protocol::MOVE_STATE_RUN);*/



		// 이전 입력과 현재 입력이 다른지 비교
		//if (DesiredInput == LastInput)
		//	SetMoveState(Protocol::MOVE_STATE_IDLE);
		//else
		//	SetMoveState(Protocol::MOVE_STATE_RUN);

		//LastInput = DesiredInput; // 현재 입력을 다음 비교를 위해 저장



		//if (!(DesiredInput == FVector2D::Zero()) && !(LastInput == DesiredInput))
		//{
		//	SetMoveState(Protocol::MOVE_STATE_RUN);
		//}
		//else if (DesiredInput == FVector2D::Zero() || DesiredInput == FVector2D::Zero())
		//{
		//	SetMoveState(Protocol::MOVE_STATE_IDLE);
		//}

		//LastInput = DesiredInput;


		// 0.1초마다 서버로 이동 패킷을 전송
	MovePacketSendTimer -= DeltaTime * 10;

	if (MovePacketSendTimer <= 0 || ForceSendPacket)
	{
		MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

		Protocol::C_MOVE MovePkt;

		// 현재 상태 정보를 패킷에 담아서 서버로 전송
		{
			Protocol::PlayerInfo* Info = MovePkt.mutable_info();
			Info->CopyFrom(*PlayerInfo);
			Info->set_yaw(DesiredYaw);
			Info->set_state(GetMoveState());

			////Info 창에 출력
			//if(GetMoveState()== Protocol::MOVE_STATE_IDLE)
			//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("IDLE")));
			//else if(GetMoveState() == Protocol::MOVE_STATE_RUN)
			//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("RUN")));


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
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

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
	bIsAiming = true;
}

void ASPCharacterPlayer::StopAiming(const FInputActionValue& Value)
{
	bIsAiming = false;
}

void ASPCharacterPlayer::Graping(const FInputActionValue& Value)
{
	if (false == bIsHolding)
	{
		FVector SphereLocationStart = Sphere->K2_GetComponentLocation();
		FVector SphereLocationEnd = SphereLocationStart + (1500 * FollowCamera->GetForwardVector());
		TArray<TEnumAsByte<EObjectTypeQuery>> EmptyObjectTypes;
		EDrawDebugTrace::Type drawDebugType = EDrawDebugTrace::ForDuration;
		TArray<AActor*> actorsToIgnore;
		FLinearColor RedColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
		FLinearColor GreenColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		float DrawTime = 5.0f;

		bool HitSuccess = GetWorld()->LineTraceSingleByChannel(outHitResult, SphereLocationStart, SphereLocationEnd, ECC_GameTraceChannel1, Params);
		//UE_LOG(LogTemp, Log, TEXT("HitSuccess %d"), HitSuccess);
		if (HitSuccess && outHitResult.Component->Mobility == EComponentMobility::Movable)
		{
			outHitResult.Component->SetSimulatePhysics(true); //시뮬레이션 켜기 
			HitComponent = outHitResult.GetComponent();
			if (HitComponent && HitComponent->IsSimulatingPhysics())
			{
				PhysicsHandleComponent->GrabComponentAtLocation(
					HitComponent,      // 잡을 컴포넌트
					NAME_None,         // 본 이름 (이 경우 빈 값)
					HitComponent->K2_GetComponentLocation() // 컴포넌트를 잡을 위치
				);
				bIsHolding = true;
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
	else // bIsHolding == true인 경우 
	{
		if (HitComponent) {
			PhysicsHandleComponent->ReleaseComponent();
			bIsHolding = false;
			HitComponent->AddImpulse(FollowCamera->GetForwardVector() * HitDistance, NAME_None, true);
			HitComponent = nullptr;
		}
	}
}

void ASPCharacterPlayer::StopGraping(const FInputActionValue& Value)
{
	if (bIsHolding && HitComponent)
	{
		bIsHolding = false;
		PhysicsHandleComponent->ReleaseComponent();
		HitComponent->AddImpulse(FollowCamera->GetForwardVector() * HitDistance, NAME_None, true);
		HitComponent = nullptr;

	}
}

//void ASPCharacterPlayer::CheckFalling()
//{
//	AActor* HitActor = outHitResult.GetActor();
//	UE_LOG(LogTemp, Log, TEXT("CheckFalling"));
//	if (HitActor)
//	{
//		FVector Location = HitActor->GetActorLocation();
//		FVector Start = Location;
//		FVector End = Location - FVector(0, 0, 100);
//		FHitResult HitResult;
//		FCollisionQueryParams Params;
//		Params.AddIgnoredActor(this);
//		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);
//		if (bHit)
//		{
//			if (HitResult.Component->ComponentHasTag(FName("Floor")))
//			{
//				UE_LOG(LogTemp, Log, TEXT("ComponentHasTag!!"));
//				outHitResult.Component->SetSimulatePhysics(false); //시뮬레이션 끄기 
//			}
//		}
//	}
//}


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

