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
#include "Weapons/WeaponDefault.h"
#include "Game/PVXGameInstance.h"

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

void AProjectVortexCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitWeapon(InitWeaponName);
}

void AProjectVortexCharacter::Move(FVector2D MovementVector)
{
	if (bIsSprintnig) return;
	ChangeMovementState(EMovementState::Run_State);
	AddMovementInput(FVector(MovementVector.X, MovementVector.Y, 0.0f));
}

void AProjectVortexCharacter::Look(FRotator NewRotation, FVector CursorLocation)
{
	SetActorRotation(FQuat(NewRotation));
	if (CurrentWeapon)
	{
		FVector Displacement = FVector(0);
		switch (MovementState) {
		case EMovementState::Aim_State:
			Displacement = FVector(0.f, 0.f, 160.f);
			CurrentWeapon->ShouldReduceDispersion = true;
			break;
		case EMovementState::Run_State:
			Displacement = FVector(0.f, 0.f, 120.f);
			CurrentWeapon->ShouldReduceDispersion = false;
			break;
		case EMovementState::Sprint_State:
			break;
		default:
			break;
		}

		CurrentWeapon->ShootEndLocation = CursorLocation + Displacement;
	}
}

void AProjectVortexCharacter::Sprint()
{
	ChangeMovementState(EMovementState::Sprint_State);
	AddMovementInput(GetActorForwardVector());
}

void AProjectVortexCharacter::Aim()
{
	ChangeMovementState(EMovementState::Aim_State);
	bIsAiming = true;
}

void AProjectVortexCharacter::AimCompleted()
{
	ChangeMovementState(EMovementState::Run_State);
	bIsAiming = false;
}

void AProjectVortexCharacter::CharacterUpdate()
{
	float ResultSpeed = 600.0f;
	switch (MovementState)
	{
	case EMovementState::Aim_State:
		ResultSpeed = MovementInfo.AimSpeed;
		break;
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
	if (CurrentWeapon)
	{
		CurrentWeapon->UpdateStateWeapon(NewMovementState);
	}
}

void AProjectVortexCharacter::InitWeapon(FName IdWeapon)
{
	UPVXGameInstance* MyGI = Cast<UPVXGameInstance>(GetGameInstance());
	FWeaponInfo MyWeaponInfo;
	if (MyGI)
	{
		if (MyGI->GetWeaponInfoByName(IdWeapon, MyWeaponInfo))
		{
			if (MyWeaponInfo.WeaponClass)
			{
				FVector SpawnLocation = FVector(0);
				FRotator SpawnRotation = FRotator(0);

				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnParams.Owner = GetOwner();
				SpawnParams.Instigator = GetInstigator();

				AWeaponDefault* MyWeapon = Cast<AWeaponDefault>(GetWorld()->SpawnActor(MyWeaponInfo.WeaponClass, &SpawnLocation, &SpawnRotation, SpawnParams));
				if (MyWeapon)
				{
					FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
					MyWeapon->AttachToComponent(GetMesh(), Rule, FName("WeaponSocketRightHand"));
					CurrentWeapon = MyWeapon;

					MyWeapon->WeaponSetting = MyWeaponInfo;
					MyWeapon->WeaponInfo.Round = MyWeaponInfo.MaxRound;

					// DEBUG !!! DELETE
					MyWeapon->ReloadTime = MyWeaponInfo.ReloadTime;

					MyWeapon->UpdateStateWeapon(MovementState);

					MyWeapon->OnWeaponReloadStart.AddDynamic(this, &AProjectVortexCharacter::WeaponReloadStart);
					MyWeapon->OnWeaponReloadEnd.AddDynamic(this, &AProjectVortexCharacter::WeaponReloadEnd);
				}
			}
		}
	}
}

AWeaponDefault* AProjectVortexCharacter::GetCurrentWeapon()
{
	return CurrentWeapon;
}

void AProjectVortexCharacter::WeaponReloadStart(UAnimMontage* Anim)
{
	WeaponReloadStart_BP(Anim);
}

void AProjectVortexCharacter::WeaponReloadEnd()
{
	WeaponReloadEnd_BP();
}

void AProjectVortexCharacter::WeaponReloadStart_BP_Implementation(UAnimMontage* Anim)
{
}

void AProjectVortexCharacter::WeaponReloadEnd_BP_Implementation()
{
}

void AProjectVortexCharacter::TryReloadWeapon()
{
	if (CurrentWeapon)
	{
		if (CurrentWeapon->GetWeaponRound() <= CurrentWeapon->WeaponSetting.MaxRound)
		{
			CurrentWeapon->InitReload();
		}
	}
}

void AProjectVortexCharacter::AttackCharEvent(bool IsAttacking)
{
	AWeaponDefault* MyWeapon = nullptr;
	MyWeapon = GetCurrentWeapon();
	if (MyWeapon)
	{
		MyWeapon->SetWeaponStateFire(IsAttacking);
	}
}