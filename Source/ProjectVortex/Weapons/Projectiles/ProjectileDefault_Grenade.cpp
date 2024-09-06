// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Projectiles/ProjectileDefault_Grenade.h"

#include "Kismet/GameplayStatics.h"

void AProjectileDefault_Grenade::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectileDefault_Grenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimerExplose(DeltaTime);
}

void AProjectileDefault_Grenade::TimerExplose(float DeltaTime)
{
	if (TimerEnabled)
	{
		if (TimerToExplose > TimeToExplose)
		{
			Explode();
		}
		else
		{
			TimerToExplose += DeltaTime;
		}
	}
}

void AProjectileDefault_Grenade::BulletCollisionSphereHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::BulletCollisionSphereHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
	// unnecessary effect appears
}

void AProjectileDefault_Grenade::ImpactProjectile()
{
	// Init grenade
	TimerEnabled = true;
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Impact"));
}

void AProjectileDefault_Grenade::InitProjectile(FProjectileInfo InitParam)
{
	Super::InitProjectile(InitParam);
	TimeToExplose = InitParam.TimeToExplose;
}

void AProjectileDefault_Grenade::Explode()
{
	//UE_LOG(LogTemp, Error, TEXT("EXPLODE"));

	TimerEnabled = false;
	if (ProjectileSetting.ExploseFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileSetting.ExploseFX, GetActorLocation(), GetActorRotation());
	}
	if (ProjectileSetting.ExploseSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileSetting.ExploseSound, GetActorLocation());
	}

	TArray<AActor*> IgnoredActor;
	UGameplayStatics::ApplyRadialDamageWithFalloff(
		GetWorld(),
		ProjectileSetting.ExploseMaxDamage,
		ProjectileSetting.ExploseMinDamage,
		GetActorLocation(),
		ProjectileSetting.MinRadiusDamage,
		ProjectileSetting.MaxRadiusDamage,
		ProjectileSetting.DamageFalloff,
		UDamageType::StaticClass(), IgnoredActor, this, nullptr
	);

	if (bShowDebug) DrawExplosionSphere();

	Destroy();
}

void AProjectileDefault_Grenade::DrawExplosionSphere()
{
	// Min damage
	DrawDebugSphere(
		GetWorld(),
		GetActorLocation(),
		ProjectileSetting.MaxRadiusDamage,
		32,
		FColor::Blue,
		false,
		2.0f
	);

	// 50% damage
	DrawDebugSphere(
		GetWorld(),
		GetActorLocation(),
		(ProjectileSetting.MaxRadiusDamage + ProjectileSetting.MinRadiusDamage) / 2.f,
		32,
		FColor::Green,
		false,
		2.0f
	);

	// Max damage
	DrawDebugSphere(
		GetWorld(),
		GetActorLocation(),
		ProjectileSetting.MinRadiusDamage,
		32,
		FColor::Red,
		false,
		2.0f
	);
}
