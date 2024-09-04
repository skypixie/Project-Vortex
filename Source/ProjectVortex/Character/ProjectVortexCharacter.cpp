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

#include "ProjectVortexPlayerController.h"
#include "Weapons/Projectiles/ProjectileDefault.h"
#include "Weapons/WeaponDefault.h"
#include "Game/PVXGameInstance.h"
#include "InventoryComponent.h"

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

	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	if (InventoryComp)
	{
		InventoryComp->OnSwitchWeapon.AddDynamic(this, &AProjectVortexCharacter::InitWeapon);
	}

	CharHealthComp = CreateDefaultSubobject<UCharacterHealthComponent>(TEXT("HealthComponent"));
	if (CharHealthComp)
	{
		CharHealthComp->OnDead.AddDynamic(this, &AProjectVortexCharacter::CharDead);
	}

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

	//InitWeapon(InitWeaponName, FAdditionalWeaponInfo());
}

void AProjectVortexCharacter::Move(FVector2D MovementVector)
{
	if (bIsSprintnig) return;
	ChangeMovementState(EMovementState::Run_State);
	AddMovementInput(FVector(MovementVector.X, MovementVector.Y, 0.0f));
}

void AProjectVortexCharacter::Look(FRotator NewRotation, FVector CursorLocation)
{
	if (!bIsAlive) return;

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

void AProjectVortexCharacter::InitWeapon(FName IdWeapon, FAdditionalWeaponInfo WeaponAdditionalInfo, int32 NewCurrentIndexWeapon)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

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
				SpawnParams.Owner = this;
				SpawnParams.Instigator = GetInstigator();

				AWeaponDefault* MyWeapon = Cast<AWeaponDefault>(GetWorld()->SpawnActor(MyWeaponInfo.WeaponClass, &SpawnLocation, &SpawnRotation, SpawnParams));
				if (MyWeapon)
				{
					FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
					MyWeapon->AttachToComponent(GetMesh(), Rule, FName("WeaponSocketRightHand"));
					CurrentWeapon = MyWeapon;

					MyWeapon->WeaponSetting = MyWeaponInfo;

					// DEBUG !!! DELETE
					MyWeapon->ReloadTime = MyWeaponInfo.ReloadTime;

					MyWeapon->UpdateStateWeapon(MovementState);

					MyWeapon->WeaponInfo = WeaponAdditionalInfo;

					CurrentIndexWeapon = NewCurrentIndexWeapon;


					MyWeapon->OnWeaponReloadStart.AddDynamic(this, &AProjectVortexCharacter::WeaponReloadStart);
					MyWeapon->OnWeaponReloadEnd.AddDynamic(this, &AProjectVortexCharacter::WeaponReloadEnd);
					MyWeapon->OnWeaponFireStart.AddDynamic(this, &AProjectVortexCharacter::WeaponFireStart);

					if (CurrentWeapon->GetWeaponRound() <= 0 && CurrentWeapon->CheckCanWeaponReload())
						CurrentWeapon->InitReload();

					if (InventoryComp)
						InventoryComp->OnWeaponAmmoAvailable.Broadcast(MyWeapon->WeaponSetting.WeaponType);
				}
			}
		}
	}
}

void AProjectVortexCharacter::WeaponFireStart_BP_Implementation(UAnimMontage* Anim)
{
}

AWeaponDefault* AProjectVortexCharacter::GetCurrentWeapon()
{
	return CurrentWeapon;
}

void AProjectVortexCharacter::WeaponReloadStart(UAnimMontage* Anim)
{
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, TEXT("WeaponReloadStart"));
	WeaponReloadStart_BP(Anim);
}

void AProjectVortexCharacter::WeaponReloadEnd(bool bIsSuccess, int32 AmmoTake)
{
	if (InventoryComp)
	{
		InventoryComp->AmmoSlotChangeValue(CurrentWeapon->WeaponSetting.WeaponType, AmmoTake);
		InventoryComp->SetAdditionalInfoWeapon(CurrentIndexWeapon, CurrentWeapon->WeaponInfo);
	}

	WeaponReloadEnd_BP(bIsSuccess);
}

void AProjectVortexCharacter::WeaponReloadStart_BP_Implementation(UAnimMontage* Anim)
{
	GetMesh()->GetAnimInstance()->Montage_Play(Anim);
}

void AProjectVortexCharacter::WeaponReloadEnd_BP_Implementation(bool bIsSuccess)
{
}

void AProjectVortexCharacter::WeaponFireStart(UAnimMontage* Anim)
{
	if (InventoryComp && CurrentWeapon)
	{
		InventoryComp->SetAdditionalInfoWeapon(CurrentIndexWeapon, CurrentWeapon->WeaponInfo);
	}

	WeaponFireStart_BP(Anim);
}

void AProjectVortexCharacter::TryReloadWeapon()
{
	if (CurrentWeapon && ! CurrentWeapon->bWeaponReloading)
	{
		if (CurrentWeapon->GetWeaponRound() < CurrentWeapon->WeaponSetting.MaxRound && CurrentWeapon->CheckCanWeaponReload())
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

		if (IsAttacking)
		{
			if (UAnimMontage* FireAnim = MyWeapon->WeaponSetting.AnimWeaponInfo.AnimCharFire)
			{
				GetMesh()->GetAnimInstance()->Montage_Play(FireAnim);
				bIsShooting = true;
			}
		}
		else
		{
			bIsShooting = false;
		}
	}
}

void AProjectVortexCharacter::TrySwitchNextWeapon()
{

	if (InventoryComp && InventoryComp->WeaponSlots.Num() > 1)
	{
		int8 OldIndex = CurrentIndexWeapon;
		FAdditionalWeaponInfo OldInfo;
		if (CurrentWeapon)
		{
			OldInfo = CurrentWeapon->WeaponInfo;
			if (CurrentWeapon->bWeaponReloading)
				CurrentWeapon->CancelReload();
		}

		if (InventoryComp->SwitchWeaponToIndex(CurrentIndexWeapon + 1, OldIndex, OldInfo, true))
		{

		}
	}
}

void AProjectVortexCharacter::TrySwitchPrevWeapon()
{
	if (InventoryComp && InventoryComp->WeaponSlots.Num() > 1)
	{
		int8 OldIndex = CurrentIndexWeapon;
		FAdditionalWeaponInfo OldInfo;
		if (CurrentWeapon)
		{
			OldInfo = CurrentWeapon->WeaponInfo;
			if (CurrentWeapon->bWeaponReloading)
				CurrentWeapon->CancelReload();
		}

		if (InventoryComp->SwitchWeaponToIndex(CurrentIndexWeapon - 1, OldIndex, OldInfo, false))
		{
		}
	}
}

void AProjectVortexCharacter::SetCurrentIndexToSwitch(int32 NewIndex)
{
	CurrentIndexToSwitch = NewIndex;
}

float AProjectVortexCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = (Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser));

	if (bIsAlive)
	{
		CharHealthComp->ChangeHealthValue(-DamageAmount);
	}

	return ActualDamage;
}

void AProjectVortexCharacter::CharDead()
{
	float TimeAnim = 0.0f;

	int32 rnd = FMath::RandHelper(DeadAnims.Num());
	if (DeadAnims.IsValidIndex(rnd) && IsValid(DeadAnims[rnd]) && GetMesh() && GetMesh()->GetAnimInstance())
	{
		TimeAnim = DeadAnims[rnd]->GetPlayLength();
		GetMesh()->GetAnimInstance()->Montage_Play(DeadAnims[rnd]);
	}

	bIsAlive = false;
	SetCanBeDamaged(false);
	UnPossessed();

	// Timer ragdoll

	GetWorldTimerManager().SetTimer(TimerHandle_RagdollTimer, this, &AProjectVortexCharacter::EnableRagdoll, TimeAnim, false);

	
}

void AProjectVortexCharacter::EnableRagdoll()
{
	if (GetMesh())
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		GetMesh()->SetSimulatePhysics(true);
	}
}
