// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UserTypes.h"
#include "ProjectVortexCharacter.generated.h"

class AWeaponDefault;

UCLASS(Blueprintable)
class AProjectVortexCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AProjectVortexCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:
	/* ----- VARIABLES ----- */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EMovementState MovementState = EMovementState::Run_State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FCharacterSpeed MovementInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsSprintnig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsAiming;

	// Stamina for sprinting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintStamina = 100.0f;

	/* --- BATTLING --- */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
	bool bIsShooting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
	float RateOfShooting = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
	TSubclassOf<class AProjectileDefault> Projectile = nullptr;

	// Weapon
	AWeaponDefault* CurrentWeapon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeaponDefault> InitWeaponClass = nullptr;

	// Timer
	float ShootTime = 0.0f;

	/* ----- FUNCTIONS ----- */

	UFUNCTION(Category = Input)
	void Move(FVector2D MovementVector);

	UFUNCTION(Category = Input)
	void Look(FRotator NewRotation);

	UFUNCTION(Category = Input)
	void Sprint();

	UFUNCTION(Category = Input)
	void Shoot();

	UFUNCTION(Category = "Input")
	void Aim();

	UFUNCTION(Category = "Input")
	void AimCompleted();

	UFUNCTION(BlueprintCallable)
	void CharacterUpdate();

	UFUNCTION(BlueprintCallable)
	void ChangeMovementState(EMovementState NewMovementState);

	UFUNCTION(BlueprintCallable)
	void SetWeaponStateShooting();

	UFUNCTION(BlueprintCallable)
	bool CheckWeaponCanShoot();

	UFUNCTION()
	void ShootTick(float DeltaTime);

	UFUNCTION()
	void InitWeapon();

	UFUNCTION(BlueprintCallable)
	AWeaponDefault* GetCurrentWeapon();
};

