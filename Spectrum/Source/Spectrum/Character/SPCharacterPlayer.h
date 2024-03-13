// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SPCharacterBase.h"
#include "InputActionValue.h"
#include "SPCharacterPlayer.generated.h"

/**
 *
 */
UCLASS()
class SPECTRUM_API ASPCharacterPlayer : public ASPCharacterBase
{
	GENERATED_BODY()

	public:
	ASPCharacterPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;	// �����ϰ� �ִ��� ���θ� �� �� ����


public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected: // ī�޶�
	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	virtual void SetCharacterControlData(const class USPCharacterControlData* CharacterControlData) override;




	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

	void QuaterMove(const FInputActionValue& Value);

	void SpeedUp(const FInputActionValue& Value);
	void StopSpeedUp(const FInputActionValue& Value);



	ECharacterControlType CurrentCharacterControlType;

protected:
	const float MOVE_PACKET_SEND_DELAY = 0.2f;				// �����Ӵ� �̵� ��Ŷ ���� ������
	float MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

	// ���������� ������ ���°�
	FVector2D DesiredInput;
	FVector DesiredMoveDirection;
	float DesiredYaw;

	// ���� ���Ⱚ(�׽�Ʈ��)
	FVector2D LastDesiredInput;


	//Camera
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UCameraComponent> FollowCamera;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuaterMoveAction;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SpeedUpAction;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;


};
