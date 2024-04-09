// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Potion/SPBlackPotion.h" 
#include "Character/SPCharacterBase.h"
#include "Interface/SPCharacterHUDInterface.h"
#include "InputActionValue.h"
#include "SPCharacterPlayer.generated.h"

/**
 *
 */
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAimingChangedDelegate, bool, bIsAiming);

UENUM()
enum class EDecalMesh : uint8
{
	SphereBlack,
	DecalBlack,
	SphereGreen,
	DecalGreen,
	SphereOrange,
	DecalOrange,
	SpherePurple,
	DecalPurple
};


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
	void CameraMove();



	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);
	void StopShoulderLook(const FInputActionValue& Value);

	void QuaterMove(const FInputActionValue& Value);

	void SpeedUp(const FInputActionValue& Value);
	void StopSpeedUp(const FInputActionValue& Value);

	void Aiming(const FInputActionValue& Value);
	void StopAiming(const FInputActionValue& Value);

	void Graping(const FInputActionValue& Value);
	void StopGraping(const FInputActionValue& Value);

	//void AimPotion(const FInputActionValue& Value);
	//void ThrowPotion(const FInputActionValue& Value);

	void Jumping(const FInputActionValue& Value);
	void StopJumping(const FInputActionValue& Value);

	//void BlackPotionSpawn(const FInputActionValue& Value);
	void GreenPotionSpawn(const FInputActionValue& Value);

	ECharacterControlType CurrentCharacterControlType;

protected:
	const float MOVE_PACKET_SEND_DELAY = 0.2f;				
	float MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

	FVector2D DesiredInput;
	FVector DesiredMoveDirection;
	float DesiredYaw;

	FVector2D LastInput;
	float LastMoveTime; 

	FVector2D LastDesiredInput;



	//Camera
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class USpringArmComponent> SpringArm;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MouseRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MouseLeft; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> BlackFour; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> GreenOne; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ThrowCtrl;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UInputAction> Test; 
protected:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UAnimMontage> ThrowMontage; 

protected:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	//uint8 bIsAiming : 1; //���� 

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	//uint8 bIsHolding : 1; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FVector UILocation; 

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	//uint8 bIsSpawn : 1; //Spawn check

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	//uint8 bIsThrowReady : 1; //Throw Ready? 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FVector UIRotator;




protected:
	UPrimitiveComponent* HitComponent;
	//AActor* HitActor;
	AActor* HitActor;
	FHitResult outHitResult;

	float HitDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UArrowComponent> GravityArrow;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Object, Meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class ASPPotionBase> Potion;

	UFUNCTION()
	void HandleMontageAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);


protected:
	void ShowProjectilePath();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USplineComponent> Projectile_Path;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", Meta = (AllowPrivateAccess = "true"))
	TArray<class USplineMeshComponent*> SplineMeshComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneComponent> PotionThrowStartLocation;

	UPROPERTY()
	//TObjectPtr<class USplineMeshComponent> SplineCoin;
	TArray<class USplineMeshComponent*> SplineCompArray;

	UPROPERTY()
	UStaticMesh* StaticMeshforSpline;

	UPROPERTY(VisibleAnywhere)
	TArray<UPrimitiveComponent*>ActorPrimitiveArray;

// Decal
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	TObjectPtr<class UDecalComponent> MyDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	TObjectPtr<class UStaticMeshComponent> DecalSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	TArray<UStaticMesh*> MeshArray;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (AllowPrivateAccess = "true"))
	//uint8 bIsDecal : 1;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (AllowPrivateAccess = "true"))
	//FVector TestDecalLocation;

};