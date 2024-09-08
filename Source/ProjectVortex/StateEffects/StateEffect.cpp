// Fill out your copyright notice in the Description page of Project Settings.

#include "StateEffects/StateEffect.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

#include "Character/HealthComponent.h"
#include "Interface/UGameActor.h"


bool UStateEffect::InitObject(AActor* Actor)
{
	myActor = Actor;

	IUGameActor* myInterface = Cast<IUGameActor>(myActor);
	if (myInterface)
	{
		myInterface->AddEffect(this);
	}

	return true;
}


void UStateEffect::DestroyObject()
{
	IUGameActor* myInterface = Cast<IUGameActor>(myActor);
	if (myInterface)
	{
		myInterface->RemoveEffect(this);
	}

	myActor = nullptr;
	this->ConditionalBeginDestroy();
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
	if (ParticleEmitter)
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

bool UStateEffect_Stun::InitObject(AActor* Actor)
{
	myActor = Actor;

	IUGameActor* myInterface = Cast<IUGameActor>(myActor);
	if (myInterface)
	{
		myInterface->AddEffect(this);
	}


	GetWorld()->GetTimerManager().SetTimer(TimerHandle_EffectTimer, this, &UStateEffect_Stun::DestroyObject, Timer, false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ExecuteTimer, this, &UStateEffect_Stun::Execute, RateTime, false);

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

void UStateEffect_Stun::DestroyObject()
{
	if (myActor)
	{
		APawn* myPawn = Cast<APawn>(myActor);
		if (myPawn && TargetController)
		{
			TargetController->Possess(myPawn);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("DESTROY"));
		}
	}

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_EffectTimer);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ExecuteTimer);

	Super::DestroyObject();
}

void UStateEffect_Stun::Execute()
{
	APawn* myPawn = Cast<APawn>(myActor);
	if (myPawn)
	{
		TargetController = myPawn->Controller;
		TargetController->UnPossess();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("EXECUTE"));
	}
}

bool UStateEffect_Immortality::InitObject(AActor* Actor)
{
	myActor = Actor;

	IUGameActor* myInterface = Cast<IUGameActor>(myActor);
	if (myInterface)
	{
		myInterface->AddEffect(this);
	}


	GetWorld()->GetTimerManager().SetTimer(TimerHandle_EffectTimer, this, &UStateEffect_Immortality::DestroyObject, Timer, false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ExecuteTimer, this, &UStateEffect_Immortality::Execute, RateTime, false);

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

void UStateEffect_Immortality::DestroyObject()
{
	if (myActor)
	{
		myActor->SetCanBeDamaged(true);
	}
	Super::DestroyObject();
}

void UStateEffect_Immortality::Execute()
{
	if (myActor)
	{
		myActor->SetCanBeDamaged(false);
	}
}
