// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "FuncLibrary/UserTypes.h"

#include "UGameActor.generated.h"

class UStateEffect;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUGameActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTVORTEX_API IUGameActor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	TArray<UStateEffect*> Effects;

	virtual EPhysicalSurface GetSurfaceType();

	virtual TArray<UStateEffect*> GetAllCurrentEffects();

	virtual void RemoveEffect(UStateEffect* Effect);

	virtual void AddEffect(UStateEffect* NewEffect);

	// inventory
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DropWeaponToWorld(FDropItem DropItemInfo);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DropAmmoToWorld(EWeaponType TypeAmmo, int32 Cout);

	// Flags
	bool bIsStunned = false;
};
