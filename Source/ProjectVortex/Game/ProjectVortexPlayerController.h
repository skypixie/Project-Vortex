// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "ProjectVortexPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;
class AProjectVortexCharacter;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class AProjectVortexPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AProjectVortexPlayerController();

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Movement Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	/** Shoot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShootAction;

	/** Aim Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimAction;

	/** Reload Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchWeaponNext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchWeaponPrev;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ActionNum1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ActionNum2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ActionNum3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ActionNum4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AbilityAction;

	UPROPERTY(BlueprintReadWrite, Category = "Character")
	AProjectVortexCharacter* PossessedPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FRotator CharacterToCursorRotation = FRotator(0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FVector CursorLocation = FVector(0);

	FTimerHandle SprintTimerHandle;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaTime) override;
	
	// To add mapping context
	virtual void BeginPlay();

	void OnMove(const FInputActionValue& Value);

	void OnSprint(const FInputActionValue& Value);

	void OnSprintCompleted(const FInputActionValue& Value);

	void OnSprintTimer();

	void OnAttackStarted(const FInputActionValue& Value);

	void OnAttackCompleted(const FInputActionValue& Value);

	void OnAim(const FInputActionValue& Value);

	void OnAimCompleted(const FInputActionValue& Value);

	void OnReload(const FInputActionValue& Value);

	void OnSwitchWeaponNext(const FInputActionValue& Value);

	void OnSwitchWeaponPrev(const FInputActionValue& Value);

	void OnActionNum1(const FInputActionValue& Value);

	void OnActionNum2(const FInputActionValue& Value);

	void OnActionNum3(const FInputActionValue& Value);

	void OnActionNum4(const FInputActionValue& Value);

	void OnAbilityAction(const FInputActionValue& Value);

	void PawnDead();

	virtual void OnUnPossess() override;
};

