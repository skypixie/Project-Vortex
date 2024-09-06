// Fill out your copyright notice in the Description page of Project Settings.

#include "StateEffects/StateEffect.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

#include "Character/HealthComponent.h"

bool UStateEffect::InitObject(AActor* Actor)
{
	myActor = Actor;

	return true;
}

void UStateEffect::ExecuteEffect(float DeltaTime)
{
}

void UStateEffect::DestroyObject()
{
	myActor = nullptr;
	this->BeginDestroy();
}

bool UStateEffect_ExecuteOnce::InitObject(AActor* Actor)
{
	Super::InitObject(Actor);
	ExecuteOnce();
	return true;
}

void UStateEffect_ExecuteOnce::DestroyObject()
{
	Super::DestroyObject();
}

void UStateEffect_ExecuteOnce::ExecuteOnce()
{
	if (myActor)
	{
		UHealthComponent* myHealthComp = Cast<UHealthComponent>(myActor->GetComponentByClass(UHealthComponent::StaticClass()));
		if (myHealthComp)
		{
			myHealthComp->ChangeHealthValue(Power);
		}

	}

	DestroyObject();
}

bool UStateEffect_ExecuteTimer::InitObject(AActor* Actor)
{
	Super::InitObject(Actor);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_EffectTimer, this, &UStateEffect_ExecuteTimer::DestroyObject, Timer, false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ExecuteTimer, this, &UStateEffect_ExecuteTimer::Execute, RateTime, true);

	if (ParticleEffect)
	{
		FName NameBoneToAttach;
		FVector Location = FVector(0);

		ParticleEmitter = UGameplayStatics::SpawnEmitterAttached(
			ParticleEffect,
			myActor->GetRootComponent(),
			NameBoneToAttach,
			Location,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			false);
	}

	return true;
}

void UStateEffect_ExecuteTimer::DestroyObject()
{
	ParticleEmitter->DestroyComponent();
	ParticleEmitter = nullptr;
	Super::DestroyObject();
}

void UStateEffect_ExecuteTimer::Execute()
{
	if (myActor)
	{
		UHealthComponent* myHealthComp = Cast<UHealthComponent>(myActor->GetComponentByClass(UHealthComponent::StaticClass()));
		if (myHealthComp)
		{
			myHealthComp->ChangeHealthValue(Power);
		}

	}
}
