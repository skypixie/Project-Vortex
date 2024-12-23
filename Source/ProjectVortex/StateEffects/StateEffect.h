// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StateEffect.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECTVORTEX_API UStateEffect : public UObject
{
	GENERATED_BODY()

public:

	virtual bool InitObject(AActor* Actor, FName NameBoneHit);
	virtual void DestroyObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TArray<TEnumAsByte<EPhysicalSurface>> PossibleInteractSurface;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	bool bIsStackable = false;

	AActor* myActor = nullptr;

};


UCLASS()
class PROJECTVORTEX_API UStateEffect_ExecuteOnce : public UStateEffect
{
	GENERATED_BODY()

public:

	virtual bool InitObject(AActor* Actor, FName NameBoneHit) override;
	virtual void DestroyObject() override;

	virtual void ExecuteOnce();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Execute Once")
	float Power = 0.0f;
};


UCLASS()
class PROJECTVORTEX_API UStateEffect_ExecuteTimer : public UStateEffect
{
	GENERATED_BODY()

public:

	FTimerHandle TimerHandle_ExecuteTimer;
	FTimerHandle TimerHandle_EffectTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Execute Timer")
	float Power = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Execute Timer")
	float Timer = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Execute Timer")
	float RateTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Execute Timer")
	UParticleSystem* ParticleEffect = nullptr;

	UParticleSystemComponent* ParticleEmitter = nullptr;

public:

	virtual bool InitObject(AActor* Actor, FName NameBoneHit) override;
	virtual void DestroyObject() override;

	virtual void Execute();
};


UCLASS()
class PROJECTVORTEX_API UStateEffect_Stun : public UStateEffect
{
	GENERATED_BODY()

public:

	FTimerHandle TimerHandle_DestroyTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Execute Timer")
	float Timer = 5.0f;

	virtual bool InitObject(AActor* Actor, FName NameBoneHit) override;
	virtual void DestroyObject() override;

	virtual void Execute();
};


UCLASS()
class PROJECTVORTEX_API UStateEffect_Immortality : public UStateEffect_ExecuteTimer
{
	GENERATED_BODY()

public:

	virtual bool InitObject(AActor* Actor, FName NameBoneHit) override;
	virtual void DestroyObject() override;

	virtual void Execute();
};