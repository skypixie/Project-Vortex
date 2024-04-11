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

#include "ProjectVortexCharacter.h"


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
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AProjectVortexPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (!IsValid(PossessedPawn))
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
		GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery3, false, HitResult);
		if (HitResult.bBlockingHit)
		{
			float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(PossessedPawn->GetActorLocation(), HitResult.Location).Yaw;
			CharacterToCursorRotation = FRotator(0.0f, FindRotatorResultYaw, 0.0f);
			// GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, TEXT("hit"));
		}
		// works incorrectly
		//else
		//{
		//	float MousePosX, MousePosY;
		//	GetMousePosition(MousePosX, MousePosY);
		//	FVector MouseWorldPosition, MouseWorldDirection;

		//	UGameplayStatics::DeprojectScreenToWorld(this, FVector2D(MousePosX, MousePosY), MouseWorldPosition, MouseWorldDirection);

		//	// GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, FString::Printf(TEXT("%f %f %f"), MouseWorldPosition.X, MouseWorldPosition.Y, MouseWorldPosition.Z));

		//	float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(PossessedPawn->GetActorLocation(), MouseWorldDirection).Yaw;
		//	NewRotation = FRotator(0.0f, FindRotatorResultYaw, 0.0f);
		//}
		PossessedPawn->Look(CharacterToCursorRotation);
	}
}
