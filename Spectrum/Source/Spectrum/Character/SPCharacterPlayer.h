// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Potion/SPBlackPotion.h" 
#include "Interface/SPCharacterHUDInterface.h"
#include "InputActionValue.h"
#include "Protocol.pb.h"
#include "SPCharacterPlayer.generated.h"

/**
 *
 */
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAimingChangedDelegate, bool, bIsAiming);

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

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
class SPECTRUM_API ASPCharacterPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	ASPCharacterPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;	// 매번 계속 해야 하는 것들 여기에


public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	virtual void SetCharacterControlData(const class USPCharacterControlData* CharacterControlData);
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

	void AimPotion(const FInputActionValue& Value);
	void ThrowPotion(const FInputActionValue& Value);

	void Jumping(const FInputActionValue& Value);
	void MyStopJumping(const FInputActionValue& Value);

	void BlackPotionSpawn(const FInputActionValue& Value);
	void GreenPotionSpawn(const FInputActionValue& Value);
	void OrangePotionSpawn(const FInputActionValue& Value);
	void PurplePotionSpawn(const FInputActionValue& Value);

	ECharacterControlType CurrentCharacterControlType;

public:
	const uint8 GetIsAiming() { return bIsAiming; };
	const uint8 GetIsHolding() { return bIsHolding; };
	const uint8 GetIsThrowReady() { return bIsThrowReady; };
	const uint8 GetIsSpawn() { return bIsSpawn; };

	void SetIsThrowReady(bool throwready) { bIsThrowReady = throwready; };
	void SetIsSpawn(bool spawn) { bIsSpawn = spawn; };

protected:
	const float MOVE_PACKET_SEND_DELAY = 0.2f;				
	float MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

	FVector2D DesiredInput;
	FVector DesiredMoveDirection;
	float DesiredYaw;

	FVector2D LastInput;
	float LastMoveTime; 

	FVector2D LastDesiredInput;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class USkeletalMeshComponent>Face;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class USkeletalMeshComponent> Torso;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class USkeletalMeshComponent> Legs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class USkeletalMeshComponent> Feet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> Staff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity Gun")
	TObjectPtr<class UPhysicsHandleComponent> PhysicsHandleComponent;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurnInPlace", Meta = (AllowPrivateAccess = "true"))
	uint8 bIsTurnRight : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurnInPlace", Meta = (AllowPrivateAccess = "true"))
	uint8 bIsTurnLeft : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurnInPlace", Meta = (AllowPrivateAccess = "true"))
	uint8 bIsTurnReady : 1; // TurnReady?

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurnInPlace", Meta = (AllowPrivateAccess = "true"))
	float PreControlYawRotation;

protected:

	

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class USPCharacterControlData*> CharacterControlManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Object, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ASPPotionBase> Potion;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	uint8 bIsAiming : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	uint8 bIsHolding : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	uint8 bIsSpawn : 1; //Spawn check

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	uint8 bIsThrowReady : 1; //Throw Ready? 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ThrowMontage;

	//Camera
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "ture"))
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
	TObjectPtr<class UInputAction> OrangeTwo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> PurpleThree; 

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

//RPC
	UFUNCTION(Server, Reliable)
	void ServerRPCSpeedUp();

	UFUNCTION(Server, Reliable)
	void ServerRPCSpeedUpStop();
	
};