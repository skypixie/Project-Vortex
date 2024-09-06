// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponDefault.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/StaticMeshActor.h"

#include "Weapons/Projectiles/ProjectileDefault.h"
#include "Character/InventoryComponent.h"
#include "StateEffects/StateEffect.h"

// Sets default values
AWeaponDefault::AWeaponDefault()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	SkeletalMeshWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMeshWeapon->SetGenerateOverlapEvents(false);
	SkeletalMeshWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	SkeletalMeshWeapon->SetupAttachment(RootComponent);

	StaticMeshWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMeshWeapon->SetGenerateOverlapEvents(false);
	StaticMeshWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	StaticMeshWeapon->SetupAttachment(RootComponent);

	ShootLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("Shoot Location"));
	ShootLocation->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeaponDefault::BeginPlay()
{
	Super::BeginPlay();
	
	WeaponInit();
}

// Called every frame
void AWeaponDefault::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FireTick(DeltaTime);
	ReloadTick(DeltaTime);
	DispersionTick(DeltaTime);
	ShellDropTick(DeltaTime);
	ClipDropTick(DeltaTime);
}

void AWeaponDefault::WeaponInit()
{
	if (SkeletalMeshWeapon && !SkeletalMeshWeapon->GetSkeletalMeshAsset())
	{
		SkeletalMeshWeapon->DestroyComponent(true);
	}

	if (StaticMeshWeapon && !StaticMeshWeapon->GetStaticMesh())
	{
		StaticMeshWeapon->DestroyComponent();
	}
}

void AWeaponDefault::SetWeaponStateFire(bool bIsFire)
{
	if (CheckWeaponCanFire())
	{
		bWeaponFiring = bIsFire;
	}
	else
	{
		bWeaponFiring = false;
		FireTimer = 0.01f;
	}
}

bool AWeaponDefault::CheckWeaponCanFire()
{
	return !bBlockFire;
}

void AWeaponDefault::FireTick(float DeltaTime)
{
	if (bWeaponFiring && GetWeaponRound() > 0 && !bWeaponReloading)
	{
		if (FireTimer < 0.f)
		{
			Fire();
		}
		else
			FireTimer -= DeltaTime;
	}
}

void AWeaponDefault::ReloadTick(float DeltaTime)
{
	if (bWeaponReloading)
	{
		if (ReloadTimer < 0.f)
		{
			FinishReload();
		}
		else
		{
			ReloadTimer -= DeltaTime;
		}
	}
}

void AWeaponDefault::DispersionTick(float DeltaTime)
{
	if (!bWeaponReloading)
	{
		if (!bWeaponFiring)
		{
			if (ShouldReduceDispersion)
				CurrentDispersion -= CurrentDispersionReduction;
			else
				CurrentDispersion += CurrentDispersionReduction;
		}

		if (CurrentDispersion < CurrentDispersionMin)
		{
			CurrentDispersion = CurrentDispersionMin;
		}
		else
		{
			if (CurrentDispersion > CurrentDispersionMax)
			{
				CurrentDispersion = CurrentDispersionMax;
			}
		}
	}
}

void AWeaponDefault::ShellDropTick(float DeltaTime)
{
	if (bDropShellFlag)
	{
		if (DropShellTimer < 0.0f)
		{
			bDropShellFlag = false;
			InitDropMesh(WeaponSetting.ShellBullets.DropMesh,
				WeaponSetting.ShellBullets.DropMeshOffset,
				WeaponSetting.ShellBullets.DropMeshImpulseDir,
				WeaponSetting.ShellBullets.DropMeshLifeTime,
				WeaponSetting.ShellBullets.ImpulseRandomDispersion,
				WeaponSetting.ShellBullets.PowerImpulse,
				WeaponSetting.ShellBullets.CustomMass);

		}
		else
			DropShellTimer -= DeltaTime;
	}
}

void AWeaponDefault::ClipDropTick(float DeltaTime)
{
	if (bDropClipFlag)
	{
		if (DropClipTimer < 0.0f)
		{
			bDropClipFlag = false;
			InitDropMesh(WeaponSetting.ClipDropMesh.DropMesh,
				WeaponSetting.ClipDropMesh.DropMeshOffset,
				WeaponSetting.ClipDropMesh.DropMeshImpulseDir,
				WeaponSetting.ClipDropMesh.DropMeshLifeTime,
				WeaponSetting.ClipDropMesh.ImpulseRandomDispersion,
				WeaponSetting.ClipDropMesh.PowerImpulse,
				WeaponSetting.ClipDropMesh.CustomMass);

		}
		else
			DropClipTimer -= DeltaTime;
	}
}

void AWeaponDefault::Fire()
{
	UAnimMontage* AnimToPlay = nullptr;
	if (bWeaponAiming)
		AnimToPlay = WeaponSetting.AnimWeaponInfo.AnimCharFireAim;
	else
		AnimToPlay = WeaponSetting.AnimWeaponInfo.AnimCharFire;

	if (WeaponSetting.AnimWeaponInfo.AnimWeaponFire
		&& SkeletalMeshWeapon
		&& SkeletalMeshWeapon->GetAnimInstance())
	{
		SkeletalMeshWeapon->GetAnimInstance()->Montage_Play(WeaponSetting.AnimWeaponInfo.AnimWeaponFire);
	}

	if (WeaponSetting.ShellBullets.DropMesh)
	{
		if (WeaponSetting.ShellBullets.DropMeshTime < 0.0f)
		{
			InitDropMesh(WeaponSetting.ShellBullets.DropMesh,
				WeaponSetting.ShellBullets.DropMeshOffset,
				WeaponSetting.ShellBullets.DropMeshImpulseDir,
				WeaponSetting.ShellBullets.DropMeshLifeTime,
				WeaponSetting.ShellBullets.ImpulseRandomDispersion,
				WeaponSetting.ShellBullets.PowerImpulse,
				WeaponSetting.ShellBullets.CustomMass);
		}
		else
		{
			bDropShellFlag = true;
			DropShellTimer = WeaponSetting.ShellBullets.DropMeshTime;
		}
	}

	FireTimer = WeaponSetting.RateOfFire;
	WeaponInfo.Round -= 1;
	ChangeDispersionByShot();

	OnWeaponFireStart.Broadcast(AnimToPlay);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponSetting.SoundFireWeapon, ShootLocation->GetComponentLocation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponSetting.EffectFireWeapon, ShootLocation->GetComponentTransform());

	int8 NumberProjectile = GetNumberProjectileByShot();

	if (ShootLocation)
	{
		FVector SpawnLocation = ShootLocation->GetComponentLocation();
		FRotator SpawnRotation = ShootLocation->GetComponentRotation();
		FProjectileInfo ProjectileInfo;
		ProjectileInfo = GetProjectile();

		FVector EndLocation;
		for (int8 i = 0; i < NumberProjectile; ++i)
		{
			EndLocation = GetFireEndLocation();

			FVector Direction = GetFireEndLocation();
			Direction.Normalize();

			FMatrix MyMatrix(Direction, FVector(0, 0, 0), FVector(0, 0, 0), FVector::ZeroVector);
			SpawnRotation = MyMatrix.Rotator();

			if (ProjectileInfo.Projectile)
			{
				/* Ballistic fire */
				if (ProjectileInfo.bIsLikeBomb)
				{

					FActorSpawnParameters SpawnParams;
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					SpawnParams.Owner = GetOwner();
					SpawnParams.Instigator = GetInstigator();

					AProjectileDefault* MyProjectile = Cast<AProjectileDefault>(GetWorld()->SpawnActor(ProjectileInfo.Projectile, &SpawnLocation, &SpawnRotation, SpawnParams));
					if (MyProjectile)
					{
						// TODO Init projectile settings by id in table row
						MyProjectile->InitProjectile(WeaponSetting.ProjectileSetting);
					}
				}
				/* Hit - Scan fire */
				else
				{
					TArray<AActor*> IgnoredActor;
					FHitResult Hit;
					

					UKismetSystemLibrary::LineTraceSingle(GetWorld(),
						SpawnLocation,
						EndLocation,
						ETraceTypeQuery::TraceTypeQuery1,
						false,
						IgnoredActor,
						EDrawDebugTrace::ForDuration,
						Hit,
						true,
						FLinearColor::Red,
						FLinearColor::Red,
						1.f);

					if (Hit.GetActor() && Hit.PhysMaterial.IsValid())
					{
						EPhysicalSurface mySurfaceType = UGameplayStatics::GetSurfaceType(Hit);

						if (WeaponSetting.ProjectileSetting.HitDecals.Contains(mySurfaceType))
						{
							UMaterialInterface* myMaterial = WeaponSetting.ProjectileSetting.HitDecals[mySurfaceType];

							if (myMaterial && Hit.GetComponent())
							{
								UGameplayStatics::SpawnDecalAttached(myMaterial, FVector(20.f), Hit.GetComponent(), NAME_None, Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), EAttachLocation::KeepWorldPosition, 10.f);
							}
						}
						if (WeaponSetting.ProjectileSetting.HitFxs.Contains(mySurfaceType))
						{
							UParticleSystem* myParticle = WeaponSetting.ProjectileSetting.HitFxs[mySurfaceType];
							if (myParticle)
							{
								UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), myParticle, FTransform(Hit.ImpactNormal.Rotation(), Hit.ImpactPoint, FVector(1.f)));
							}
						}
						if (WeaponSetting.ProjectileSetting.HitSound)
						{
							UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponSetting.ProjectileSetting.HitSound, Hit.ImpactPoint);
						}
						//UGameplayStatics::ApplyDamage(Hit.GetActor(), WeaponSetting.ProjectileSetting.ProjectileDamage, GetInstigatorController(), this, NULL);
						
						UUserTypes::AddEffectSurfaceType(Hit.GetActor(), ProjectileInfo.Effect, mySurfaceType);

						UGameplayStatics::ApplyPointDamage(Hit.GetActor(), WeaponSetting.ProjectileSetting.ProjectileDamage, Hit.TraceStart, Hit, GetInstigatorController(), this, NULL);


					}
				}
			}

			bDropShellFlag = true;
		}
	}

	if (GetWeaponRound() <= 0 && !bWeaponReloading)
	{
		if (CheckCanWeaponReload())
			InitReload();
	}
}

FProjectileInfo AWeaponDefault::GetProjectile()
{
	return WeaponSetting.ProjectileSetting;
}

int32 AWeaponDefault::GetWeaponRound()
{
	return WeaponInfo.Round;
}

void AWeaponDefault::InitReload()
{
	bWeaponReloading = true;

	ReloadTimer = WeaponSetting.ReloadTime;

	if (bWeaponAiming)
	{
		UAnimMontage* AnimMontage = WeaponSetting.AnimWeaponInfo.AnimCharReloadAim;
		if (AnimMontage)
			OnWeaponReloadStart.Broadcast(AnimMontage);
	}
	else
	{
		UAnimMontage* AnimMontage = WeaponSetting.AnimWeaponInfo.AnimCharReload;
		if (AnimMontage)
		{
			OnWeaponReloadStart.Broadcast(AnimMontage);
		}
	}

	if (WeaponSetting.ClipDropMesh.DropMesh)
	{
		bDropClipFlag = true;
		DropClipTimer = WeaponSetting.ClipDropMesh.DropMeshTime;
	}
}

void AWeaponDefault::FinishReload()
{
	bWeaponReloading = false;

	int8 AvailableAmmoFromInventory = GetAvailableAmmoForReload();
	int8 AmmoToTake;
	int8 NeedToReload = WeaponSetting.MaxRound - WeaponInfo.Round;

	if (NeedToReload > AvailableAmmoFromInventory)
	{
		WeaponInfo.Round = AvailableAmmoFromInventory;
		AmmoToTake = AvailableAmmoFromInventory;
	}
	else
	{
		WeaponInfo.Round += NeedToReload;
		AmmoToTake = NeedToReload;
	}

	OnWeaponReloadEnd.Broadcast(true, -AmmoToTake);
}

bool AWeaponDefault::CheckCanWeaponReload()
{
	bool result = true;
	if (GetOwner())
	{
		UE_LOG(LogTemp, Error, TEXT("AWeaponDefault::CheckCanWeaponReload - OwnerName - %s"), *GetOwner()->GetName());

		UInventoryComponent* MyInv = Cast<UInventoryComponent>(GetOwner()->GetComponentByClass(UInventoryComponent::StaticClass()));
		if (MyInv)
		{
			int8 AvailableAmmoForWeapon;
			if (!MyInv->CheckAmmoForWeapon(WeaponSetting.WeaponType, AvailableAmmoForWeapon))
			{
				result = false;
			}
		}
	}
	return result;
}

int8 AWeaponDefault::GetAvailableAmmoForReload()
{
	int8 AvailableAmmoForWeapon = WeaponSetting.MaxRound;
	if (GetOwner())
	{
		UE_LOG(LogTemp, Error, TEXT("AWeaponDefault::CheckCanWeaponReload - OwnerName - %s"), *GetOwner()->GetName());

		UInventoryComponent* MyInv = Cast<UInventoryComponent>(GetOwner()->GetComponentByClass(UInventoryComponent::StaticClass()));
		if (MyInv)
		{
			if (MyInv->CheckAmmoForWeapon(WeaponSetting.WeaponType, AvailableAmmoForWeapon))
			{
				
			}
		}
	}
	return AvailableAmmoForWeapon;
}

void AWeaponDefault::InitDropMesh(UStaticMesh* DropMesh, FTransform Offset, FVector DropImpulseDirection, float LifeTimeMesh, float ImpulseRandomDispersion, float PowerImpulse, float CustomMass)
{
	if (!DropMesh) return;

	FTransform Transform;
	FVector LocalDir = GetActorForwardVector() * Offset.GetLocation().X + GetActorRightVector() * Offset.GetLocation().Y + GetActorUpVector() * Offset.GetLocation().Z;

	Transform.SetLocation(GetActorLocation() + LocalDir);
	Transform.SetScale3D(Offset.GetScale3D());

	Transform.SetRotation((GetActorRotation() + Offset.Rotator()).Quaternion());
	AStaticMeshActor* NewActor = nullptr;

	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	Param.Owner = this;

	NewActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Transform, Param);

	if (NewActor && NewActor->GetStaticMeshComponent())
	{
		NewActor->GetStaticMeshComponent()->SetCollisionProfileName(TEXT("BlockAll"));
		NewActor->GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		NewActor->SetActorTickEnabled(false);
		NewActor->SetLifeSpan(LifeTimeMesh);

		NewActor->GetStaticMeshComponent()->Mobility = EComponentMobility::Movable;
		NewActor->GetStaticMeshComponent()->SetSimulatePhysics(true);
		NewActor->GetStaticMeshComponent()->SetStaticMesh(DropMesh);

		// doesnt work if custom mass is not set idk why
		if (CustomMass >= 0.0f)
		{
			NewActor->GetStaticMeshComponent()->SetMassOverrideInKg(NAME_None, CustomMass, true);
		}

		if (!DropImpulseDirection.IsNearlyZero())
		{
			FVector FinalDir;
			FinalDir = LocalDir + (DropImpulseDirection * 100.0f);

			if (!FMath::IsNearlyZero(ImpulseRandomDispersion))
				FinalDir += UKismetMathLibrary::RandomUnitVectorInConeInDegrees(LocalDir, ImpulseRandomDispersion);
			FinalDir.GetSafeNormal(0.0001f);

			NewActor->GetStaticMeshComponent()->AddImpulse(FinalDir * PowerImpulse);
		}
	}
}

void AWeaponDefault::UpdateStateWeapon(EMovementState NewMovementState)
{
	bBlockFire = false;

	switch (NewMovementState)
	{
	case EMovementState::Aim_State:
		CurrentDispersionMax = WeaponSetting.DispersionWeapon.Aim_StateDispersionAimMax;
		CurrentDispersionMin = WeaponSetting.DispersionWeapon.Aim_StateDispersionAimMin;
		CurrentDispersionRecoil = WeaponSetting.DispersionWeapon.Aim_StateDispersionAimRecoil;
		CurrentDispersionReduction = WeaponSetting.DispersionWeapon.Aim_StateDispersionReduction;
		bWeaponAiming = true;
		break;
	case EMovementState::Run_State:
		CurrentDispersionMax = WeaponSetting.DispersionWeapon.Run_StateDispersionAimMax;
		CurrentDispersionMin = WeaponSetting.DispersionWeapon.Run_StateDispersionAimMin;
		CurrentDispersionRecoil = WeaponSetting.DispersionWeapon.Run_StateDispersionAimRecoil;
		CurrentDispersionReduction = WeaponSetting.DispersionWeapon.Run_StateDispersionReduction;
		bWeaponAiming = false;
		break;
	case EMovementState::Sprint_State:
		bBlockFire = true;
		bWeaponAiming = false;
		SetWeaponStateFire(false);
		break;
	default:
		break;
	}
}

void AWeaponDefault::ChangeDispersionByShot()
{
	CurrentDispersion += CurrentDispersionRecoil;
}

float AWeaponDefault::GetCurrentDispersion() const
{
	return CurrentDispersion;
}

FVector AWeaponDefault::ApplyDispersionToShoot(FVector DirectionShoot) const
{
	return FMath::VRandCone(DirectionShoot, GetCurrentDispersion() * PI / 180.f);
}

FVector AWeaponDefault::GetFireEndLocation() const
{
	FVector EndLocation = FVector(0.f);

	FVector tmpV = (ShootLocation->GetComponentLocation() - ShootEndLocation);
	if (tmpV.Size() > SizeVectorToChangeShootDirectionLogic)
	{
		EndLocation = ShootLocation->GetComponentLocation() + ApplyDispersionToShoot((ShootLocation->GetComponentLocation() - ShootEndLocation).GetSafeNormal()) * -20000.0f;
		if (ShowDebug)
			DrawDebugCone(GetWorld(), ShootLocation->GetComponentLocation(), -(ShootLocation->GetComponentLocation() - ShootEndLocation), WeaponSetting.DistanceTrace, GetCurrentDispersion() * PI / 180.f, GetCurrentDispersion() * PI / 180.f, 32, FColor::Emerald, false, 0.1f, (uint8)'\000', 1.0f);
	}
	else
	{
		EndLocation = ShootLocation->GetComponentLocation() + ApplyDispersionToShoot(ShootLocation->GetForwardVector()) * 20000.0f;
	}

	//if (byBarrel)
	//{
	//	EndLocation = ShootLocation->GetComponentLocation() + ApplyDispersionToShoot((ShootLocation->GetComponentLocation() - ShootEndLocation).GetSafeNormal()) * -20000.0f;
	//	DrawDebugCone(GetWorld(), ShootLocation->GetComponentLocation(), -(ShootLocation->GetComponentLocation() - ShootEndLocation), WeaponSetting.DistanceTrace, GetCurrentDispersion() * PI / 180.f, GetCurrentDispersion() * PI / 180.f, 32, FColor::Emerald, false, 0.1f, (uint8)'\000', 1.0f);
	//}

	return EndLocation;
}

int8 AWeaponDefault::GetNumberProjectileByShot() const
{
	return WeaponSetting.NumberProjectileByShot;
}

void AWeaponDefault::CancelReload()
{
	bWeaponReloading = false;
	if (SkeletalMeshWeapon && SkeletalMeshWeapon->GetAnimInstance())
		SkeletalMeshWeapon->GetAnimInstance()->StopAllMontages(0.15f);

	OnWeaponReloadEnd.Broadcast(false, 0);
	bDropClipFlag = false;
}
