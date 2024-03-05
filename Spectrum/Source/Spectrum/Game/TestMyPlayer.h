// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/TestPlayer.h"
#include "InputActionValue.h"
#include "TestMyPlayer.generated.h"

/**
 * 내가 컨트롤하는 플레이어만 가지고 있어야 하는 부분
 */
UCLASS()
class SPECTRUM_API ATestMyPlayer : public ATestPlayer
{
	GENERATED_BODY()
	
public:
	ATestMyPlayer();



protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;	// 존재하고 있는지 여부를 알 수 있음


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


protected:

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);




protected:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;


protected:
	const float MOVE_PACKET_SEND_DELAY = 0.2f;				// 프레임당 이동 패킷 전송 딜레이
	float MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

	// 최종적으로 전송할 상태값
	FVector2D DesiredInput;
	FVector DesiredMoveDirection;
	float DesiredYaw;

	// 직전 방향값(테스트용)
	FVector2D LastDesiredInput;

};
