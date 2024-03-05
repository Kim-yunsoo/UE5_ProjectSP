// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/TestPlayer.h"
#include "InputActionValue.h"
#include "TestMyPlayer.generated.h"

/**
 * ���� ��Ʈ���ϴ� �÷��̾ ������ �־�� �ϴ� �κ�
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

	virtual void Tick(float DeltaTime) override;	// �����ϰ� �ִ��� ���θ� �� �� ����


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
	const float MOVE_PACKET_SEND_DELAY = 0.2f;				// �����Ӵ� �̵� ��Ŷ ���� ������
	float MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

	// ���������� ������ ���°�
	FVector2D DesiredInput;
	FVector DesiredMoveDirection;
	float DesiredYaw;

	// ���� ���Ⱚ(�׽�Ʈ��)
	FVector2D LastDesiredInput;

};
