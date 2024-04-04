// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UserTypes.h"
#include "ProjectVortexCharacter.generated.h"

UCLASS(Blueprintable)
class AProjectVortexCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AProjectVortexCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

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

	/* ----- FUNCTIONS ----- */

	UFUNCTION(Category = Input)
	void Move(const FInputActionValue& Value);

	UFUNCTION(Category = Input)
	void Look(FRotator NewRotation);

	UFUNCTION(BlueprintCallable)
	void CharacterUpdate();

	UFUNCTION(BlueprintCallable)
	void ChangeMovementState(EMovementState NewMovementState);
};

