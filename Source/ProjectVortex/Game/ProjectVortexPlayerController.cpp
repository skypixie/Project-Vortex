// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectVortexPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Character/ProjectVortexCharacter.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AProjectVortexPlayerController::AProjectVortexPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AProjectVortexPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	PossessedPawn = Cast<AProjectVortexCharacter>(GetPawn());

	GetWorld()->GetTimerManager().SetTimer(SprintTimerHandle, this, &AProjectVortexPlayerController::OnSprintTimer, 0.1f, true);
	// Print trace channels names
	/*
	for (int32 i = 0; i < ETraceTypeQuery::TraceTypeQuery_MAX; ++i)
	{
		ETraceTypeQuery EnumValue = static_cast<ETraceTypeQuery>(i);
		FString EnumName = UEnum::GetValueAsString(EnumValue);
		FString ChannelName = UEnum::GetValueAsString(UEngineTypes::ConvertToCollisionChannel(static_cast<ETraceTypeQuery>(i)));
		UE_LOG(LogTemp, Log, TEXT("ETraceTypeQuery Name: %s"), *EnumName);
		UE_LOG(LogTemp, Log, TEXT("Collision Channel Name: %s"), *ChannelName);
	}
	for (int32 i = 0; i < EObjectTypeQuery::ObjectTypeQuery_MAX; ++i)
	{
		EObjectTypeQuery EnumValue = static_cast<EObjectTypeQuery>(i);
		FString EnumName = UEnum::GetValueAsString(EnumValue);
		FString ChannelName = UEnum::GetValueAsString(UEngineTypes::ConvertToCollisionChannel(static_cast<EObjectTypeQuery>(i)));
		UE_LOG(LogTemp, Log, TEXT("EObjectTypeQuery Name: %s"), *EnumName);
		UE_LOG(LogTemp, Log, TEXT("Collision Channel Name: %s"), *ChannelName);
	}
	*/
}

void AProjectVortexPlayerController::OnMove(const FInputActionValue& Value)
{
	if (IsValid(PossessedPawn))
	{
		PossessedPawn->Move(Value.Get<FVector2D>());
	}
}

void AProjectVortexPlayerController::OnSprint(const FInputActionValue& Value)
{
	if (IsValid(PossessedPawn) && PossessedPawn->SprintStamina)
	{
		PossessedPawn->bIsSprintnig = true;
		PossessedPawn->Sprint();
	}
}

void AProjectVortexPlayerController::OnSprintCompleted(const FInputActionValue& Value)
{
	if (IsValid(PossessedPawn))
	{
		PossessedPawn->bIsSprintnig = false;
	}
}

void AProjectVortexPlayerController::OnSprintTimer()
{
	if (IsValid(PossessedPawn))
	{
		if (PossessedPawn->SprintStamina > 0 && PossessedPawn->bIsSprintnig)
		{
			PossessedPawn->SprintStamina -= 1;
		}
		else if (PossessedPawn->SprintStamina >= 0 && (!PossessedPawn->bIsSprintnig) && PossessedPawn->SprintStamina < 100)
		{
			PossessedPawn->SprintStamina += 1;
		}


		// GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, FString::Printf(TEXT("%f"), PossessedPawn->SprintStamina));
	}
}

void AProjectVortexPlayerController::OnAttackStarted(const FInputActionValue& Value)
{
	if (IsValid(PossessedPawn))
	{
		PossessedPawn->AttackCharEvent(true);
	}
}

void AProjectVortexPlayerController::OnAttackCompleted(const FInputActionValue& Value)
{
	if (IsValid(PossessedPawn))
	{
		PossessedPawn->AttackCharEvent(false);
	}
}

void AProjectVortexPlayerController::OnAim(const FInputActionValue& Value)
{
	if (IsValid(PossessedPawn))
	{
		PossessedPawn->Aim();
	}
}

void AProjectVortexPlayerController::OnAimCompleted(const FInputActionValue& Value)
{
	if (IsValid(PossessedPawn))
	{
		PossessedPawn->AimCompleted();
	}
}

void AProjectVortexPlayerController::OnReload(const FInputActionValue& Value)
{
	if (IsValid(PossessedPawn))
	{
		PossessedPawn->TryReloadWeapon();
	}
}

void AProjectVortexPlayerController::OnSwitchWeaponNext(const FInputActionValue& Value)
{
	if (IsValid(PossessedPawn))
	{
		PossessedPawn->TrySwitchNextWeapon();
	}
}

void AProjectVortexPlayerController::OnSwitchWeaponPrev(const FInputActionValue& Value)
{
	if (IsValid(PossessedPawn))
	{
		PossessedPawn->TrySwitchPrevWeapon();
	}
}

void AProjectVortexPlayerController::OnActionNum1(const FInputActionValue& Value)
{
	if (IsValid(PossessedPawn))
	{
		PossessedPawn->SetCurrentIndexToSwitch(0);
	}
}

void AProjectVortexPlayerController::OnActionNum2(const FInputActionValue& Value)
{
	if (IsValid(PossessedPawn))
	{
		PossessedPawn->SetCurrentIndexToSwitch(1);
	}
}

void AProjectVortexPlayerController::OnActionNum3(const FInputActionValue& Value)
{
	if (IsValid(PossessedPawn))
	{
		PossessedPawn->SetCurrentIndexToSwitch(2);
	}
}

void AProjectVortexPlayerController::OnActionNum4(const FInputActionValue& Value)
{
	if (IsValid(PossessedPawn))
	{
		PossessedPawn->SetCurrentIndexToSwitch(3);
	}
}

void AProjectVortexPlayerController::OnAbilityAction(const FInputActionValue& Value)
{
	if (IsValid(PossessedPawn))
	{
		PossessedPawn->TryAbilityEnabled();
	}
}

void AProjectVortexPlayerController::PawnDead()
{

}

void AProjectVortexPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup jogging
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProjectVortexPlayerController::OnMove);
		// Setup sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AProjectVortexPlayerController::OnSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AProjectVortexPlayerController::OnSprintCompleted);
		// Setup Battling
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AProjectVortexPlayerController::OnAttackStarted);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &AProjectVortexPlayerController::OnAttackCompleted);

		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &AProjectVortexPlayerController::OnAim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AProjectVortexPlayerController::OnAimCompleted);

		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AProjectVortexPlayerController::OnReload);

		EnhancedInputComponent->BindAction(SwitchWeaponNext, ETriggerEvent::Started, this, &AProjectVortexPlayerController::OnSwitchWeaponNext);
		EnhancedInputComponent->BindAction(SwitchWeaponPrev, ETriggerEvent::Started, this, &AProjectVortexPlayerController::OnSwitchWeaponPrev);

		EnhancedInputComponent->BindAction(ActionNum1, ETriggerEvent::Started, this, &AProjectVortexPlayerController::OnActionNum1);
		EnhancedInputComponent->BindAction(ActionNum2, ETriggerEvent::Started, this, &AProjectVortexPlayerController::OnActionNum2);
		EnhancedInputComponent->BindAction(ActionNum3, ETriggerEvent::Started, this, &AProjectVortexPlayerController::OnActionNum3);
		EnhancedInputComponent->BindAction(ActionNum4, ETriggerEvent::Started, this, &AProjectVortexPlayerController::OnActionNum4);

		EnhancedInputComponent->BindAction(AbilityAction, ETriggerEvent::Started, this, &AProjectVortexPlayerController::OnAbilityAction);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AProjectVortexPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (IsValid(InPawn))
	{
		PossessedPawn = Cast<AProjectVortexCharacter>(InPawn);
	}
}

void AProjectVortexPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(PossessedPawn))
	{
		FVector PawnLocation = PossessedPawn->GetActorLocation();
		FHitResult HitResult;
		GetHitResultUnderCursor(ECC_GameTraceChannel1, true, HitResult);
		if (HitResult.bBlockingHit)
		{
			float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(PossessedPawn->GetActorLocation(), HitResult.Location).Yaw;
			CharacterToCursorRotation = FRotator(0.0f, FindRotatorResultYaw, 0.0f);
			CursorLocation = HitResult.Location;
			// GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, TEXT("hit"));
		}
		PossessedPawn->Look(CharacterToCursorRotation, CursorLocation);
	}
}

void AProjectVortexPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	
}