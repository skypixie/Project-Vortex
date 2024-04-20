// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectVortexCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "InputActionValue.h"

#include "Weapons/Projectiles/ProjectileDefault.h"

AProjectVortexCharacter::AProjectVortexCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AProjectVortexCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AProjectVortexCharacter::Move(FVector2D MovementVector)
{
	if (bIsSprintnig) return;
	ChangeMovementState(EMovementState::Run_State);
	AddMovementInput(FVector(MovementVector.X, MovementVector.Y, 0.0f));
}

void AProjectVortexCharacter::Look(FRotator NewRotation)
{
	SetActorRotation(FQuat(NewRotation));
}

void AProjectVortexCharacter::Sprint()
{
	ChangeMovementState(EMovementState::Sprint_State);
	AddMovementInput(GetActorForwardVector());
}

void AProjectVortexCharacter::Shoot()
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, TEXT("shooting"));
	ShootTime = RateOfShooting;
	
	FVector ShootLocation = GetActorForwardVector();
	FVector SpawnLocation = GetMesh()->GetSocketLocation(TEXT("BulletSocket"));
	FRotator SpawnRotation = GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	AProjectileDefault* MyProjectile = Cast<AProjectileDefault>(GetWorld()->SpawnActor(Projectile, &SpawnLocation, &SpawnRotation, SpawnParams));
	if (MyProjectile)
	{
		MyProjectile->InitialLifeSpan = 20.f;
	}
}

void AProjectVortexCharacter::CharacterUpdate()
{
	float ResultSpeed = 600.0f;
	switch (MovementState)
	{
	case EMovementState::Run_State:
		ResultSpeed = MovementInfo.RunSpeed;
		break;
	case EMovementState::Sprint_State:
		ResultSpeed = MovementInfo.SprintSpeed;
		break;
	default:
		break;
	}

	GetCharacterMovement()->MaxWalkSpeed = ResultSpeed;
}

void AProjectVortexCharacter::ChangeMovementState(EMovementState NewMovementState)
{
	MovementState = NewMovementState;
	CharacterUpdate();
}

void AProjectVortexCharacter::SetWeaponStateShooting()
{
	if (!CheckWeaponCanShoot())
	{
		bIsShooting = false;
	}
}

bool AProjectVortexCharacter::CheckWeaponCanShoot()
{
	return true;
}

void AProjectVortexCharacter::ShootTick(float DeltaTime)
{
	if (bIsShooting)
	{
		if (ShootTime < 0.f) Shoot();
		else ShootTime -= DeltaTime;
	}
}
