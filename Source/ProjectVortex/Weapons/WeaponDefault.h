// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"

#include "FuncLibrary/UserTypes.h"

#include "WeaponDefault.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponReloadStart, UAnimMontage*, Anim);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponReloadEnd, bool, bIsSuccess, int32, AmmoSave);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponFireStart, UAnimMontage*, Anim);

UCLASS()
class PROJECTVORTEX_API AWeaponDefault : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponDefault();

	FOnWeaponReloadStart OnWeaponReloadStart;
	FOnWeaponReloadEnd OnWeaponReloadEnd;
	FOnWeaponFireStart OnWeaponFireStart;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	class USceneComponent* SceneComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	class USkeletalMeshComponent* SkeletalMeshWeapon = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	class UStaticMeshComponent* StaticMeshWeapon = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	class UArrowComponent* ShootLocation = nullptr;

	UPROPERTY()
	FWeaponInfo WeaponSetting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FAdditionalWeaponInfo WeaponInfo;

	// Flags
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Logic")
	bool bWeaponFiring = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Logic")
	bool bWeaponAiming = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Logic")
	bool bBlockFire = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload Logic")
	bool bWeaponReloading = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop Mesh Logic")
	bool bDropShellFlag = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop Mesh Logic")
	bool bDropClipFlag = false;

	// Timer
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Logic")
	float FireTimer = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload Logic")
	float ReloadTimer = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop Mesh Logic")
	float DropShellTimer = -1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop Mesh Logic")
	float DropClipTimer = -1.0f;

	// Dispersion
	bool ShouldReduceDispersion = false;
	float CurrentDispersion = 0.0f;
	float CurrentDispersionMax = 1.0f;
	float CurrentDispersionMin = 0.1f;
	float CurrentDispersionRecoil = 0.1f;
	float CurrentDispersionReduction = 0.1f;

	FVector ShootEndLocation = FVector(0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	float SizeVectorToChangeShootDirectionLogic = 300.f;

	// DELETE !!! DEBUG
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload Logic Debug")
	float ReloadTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool ShowDebug = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void WeaponInit();

	UFUNCTION(BlueprintCallable)
	void SetWeaponStateFire(bool bIsFire);

	UFUNCTION(BlueprintCallable)
	bool CheckWeaponCanFire();

	void FireTick(float DeltaTime);

	void ReloadTick(float DeltaTime);

	void DispersionTick(float DeltaTime);

	void ShellDropTick(float DeltaTime);

	void ClipDropTick(float DeltaTime);

	void Fire();

	FProjectileInfo GetProjectile();

	UFUNCTION(BlueprintCallable)
	int32 GetWeaponRound();

	UFUNCTION()
	void InitReload();

	UFUNCTION()
	void FinishReload();

	UFUNCTION()
	bool CheckCanWeaponReload();

	UFUNCTION()
	int8 GetAvailableAmmoForReload();

	UFUNCTION()
	void InitDropMesh(UStaticMesh* DropMesh, FTransform Offset, FVector DropImpulseDirection, float LifeTimeMesh, float ImpulseRandomDispersion, float PowerImpulse, float CustomMass);

	void UpdateStateWeapon(EMovementState NewMovementState);
	void ChangeDispersionByShot();
	float GetCurrentDispersion() const;
	FVector ApplyDispersionToShoot(FVector DirectionShoot) const;
	FVector GetFireEndLocation() const;
	int8 GetNumberProjectileByShot() const;

	void CancelReload();
};
