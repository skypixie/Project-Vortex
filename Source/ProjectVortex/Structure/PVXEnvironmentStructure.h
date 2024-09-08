// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interface/UGameActor.h"
#include "StateEffects/StateEffect.h"

#include "PVXEnvironmentStructure.generated.h"

UCLASS()
class PROJECTVORTEX_API APVXEnvironmentStructure : public AActor, public IUGameActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APVXEnvironmentStructure();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UStateEffect*> Effects;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	EPhysicalSurface GetSurfaceType() override;

	TArray<UStateEffect*> GetAllCurrentEffects() override;

	void RemoveEffect(UStateEffect* Effect) override;

	void AddEffect(UStateEffect* NewEffect) override;

};
