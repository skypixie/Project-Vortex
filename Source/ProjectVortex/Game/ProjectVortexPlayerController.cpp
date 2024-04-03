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
}

void AProjectVortexPlayerController::OnMove(const FInputActionValue& Value)
{
	if (IsValid(PossessedPawn))
	{
		PossessedPawn->Move(Value);
	}
}

void AProjectVortexPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup keyboard movement
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProjectVortexPlayerController::OnMove);
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

	FHitResult HitResult;
	GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery6, false, HitResult);
	float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(PossessedPawn->GetActorLocation(), HitResult.Location).Yaw;
	FRotator NewRotation = FRotator(0.0f, FindRotatorResultYaw, 0.0f);
	PossessedPawn->Look(NewRotation);
}
