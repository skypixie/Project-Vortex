// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


#include "ProjectileDefault.generated.h"

UCLASS()
class PROJECTVORTEX_API AProjectileDefault : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileDefault();

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* BulletMesh = nullptr;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* BulletCollisionSphere = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UProjectileMovementComponent* BulletProjectileMovement = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UParticleSystemComponent* BulletFX = nullptr;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void BulletCollisionSphereHit(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};