// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "FuncLibrary/UserTypes.h"
#include "CharacterHealthComponent.h"
#include "Interface/UGameActor.h"

#include "ProjectVortexCharacter.generated.h"

class AWeaponDefault;
class UInventoryComponent;

UCLASS(Blueprintable)
class AProjectVortexCharacter : public ACharacter, public IUGameActor
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCharacterHealthComponent* CharHealthComp;

public:

	FTimerHandle TimerHandle_RagdollTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EMovementState MovementState = EMovementState::Run_State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FCharacterSpeed MovementInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsSprintnig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsAiming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsAlive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	TArray<UAnimMontage*> DeadAnims;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<UStateEffect> AbilityEffect;

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

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 CurrentIndexWeapon = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demo")
	FName InitWeaponName;

	// PickUp Logic
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickUps")
	int32 CurrentIndexToSwitch;

	// Timer
	float ShootTime = 0.0f;

	TArray<UStateEffect*> Effects;

public:

	/* ----- FUNCTIONS ----- */

	UFUNCTION(Category = Input)
	void Move(FVector2D MovementVector);

	UFUNCTION(Category = Input)
	void Look(FRotator NewRotation, FVector CursorLocation);

	UFUNCTION(Category = Input)
	void Sprint();

	UFUNCTION(Category = "Input")
	void Aim();

	UFUNCTION(Category = "Input")
	void AimCompleted();

	UFUNCTION(BlueprintCallable)
	void CharacterUpdate();
	
	UFUNCTION(BlueprintCallable)
	void ChangeMovementState(EMovementState NewMovementState);

	UFUNCTION()
	void InitWeapon(FName IdWeapon, FAdditionalWeaponInfo WeaponAdditionalInfo, int32 NewCurrentIndexWeapon);

	UFUNCTION(BlueprintCallable)
	void TryReloadWeapon();

	UFUNCTION()
	void WeaponReloadStart(UAnimMontage* Anim);

	UFUNCTION()
	void WeaponReloadEnd(bool bIsSuccess, int32 AmmoTake);

	UFUNCTION(BlueprintNativeEvent)
	void WeaponReloadStart_BP(UAnimMontage* Anim);
	void WeaponReloadStart_BP_Implementation(UAnimMontage* Anim);

	UFUNCTION(BlueprintNativeEvent)
	void WeaponReloadEnd_BP(bool bIsSuccess);
	void WeaponReloadEnd_BP_Implementation(bool bIsSuccess);

	UFUNCTION()
	void WeaponFireStart(UAnimMontage* Anim);
	UFUNCTION(BlueprintNativeEvent)
	void WeaponFireStart_BP(UAnimMontage* Anim);
	void WeaponFireStart_BP_Implementation(UAnimMontage* Anim);

	UFUNCTION(BlueprintCallable)
	AWeaponDefault* GetCurrentWeapon();


	UFUNCTION(BlueprintCallable)
	void AttackCharEvent(bool IsAttacking);

	UFUNCTION(BlueprintCallable)
	void TryAbilityEnabled();

	UFUNCTION(BlueprintCallable)
	void TrySwitchNextWeapon();


	UFUNCTION(BlueprintCallable)
	void TrySwitchPrevWeapon();

	UFUNCTION(BlueprintCallable)
	void SetCurrentIndexToSwitch(int32 NewIndex);

	// ===== INTERFACE =====

	virtual EPhysicalSurface GetSurfaceType() override;

	TArray<UStateEffect*> GetAllCurrentEffects() override;

	void RemoveEffect(UStateEffect* Effect) override;

	void AddEffect(UStateEffect* NewEffect) override;

	// =====================

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	void CharDead();

	void EnableRagdoll();
};

