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
#include "Kismet/KismetMathLibrary.h"



ASPCharacterPlayer::ASPCharacterPlayer()
{

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// 카메라 시점에 따른 에셋	로드
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
	CurrentCharacterControlType = ECharacterControlType::Shoulder;
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
	if (DesiredInput == FVector2D::Zero())
		SetMoveState(Protocol::MOVE_STATE_IDLE);
	else
		SetMoveState(Protocol::MOVE_STATE_RUN);


	// 0.1초마다 서버로 이동 패킷을 전송
	MovePacketSendTimer -= DeltaTime;

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
		}

		SEND_PACKET(MovePkt);
	}


}

void ASPCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);


	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &ASPCharacterPlayer::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &ASPCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &ASPCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &ASPCharacterPlayer::QuaterMove);
	EnhancedInputComponent->BindAction(SpeedUpAction, ETriggerEvent::Triggered, this, &ASPCharacterPlayer::SpeedUp);
	EnhancedInputComponent->BindAction(SpeedUpAction, ETriggerEvent::Completed, this, &ASPCharacterPlayer::StopSpeedUp);

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
			DesiredMoveDirection += ForwardDirection * MovementVector.Y;
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
	GetCharacterMovement()->MaxWalkSpeed = 900.f;
}

void ASPCharacterPlayer::StopSpeedUp(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
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

