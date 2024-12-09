// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/HealthComponent.h"
#include "CharacterHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShieldChange, float, Shield, float, Damage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShieldBreak);

/**
 * 
 */
UCLASS()
class PROJECTVORTEX_API UCharacterHealthComponent : public UHealthComponent
{
	GENERATED_BODY()

public:

	FTimerHandle TimerHandle_CooldownShieldTimer;

	FTimerHandle TimerHandle_ShieldRecoveryRateTimer;

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Shield")
	FOnShieldChange OnShieldChange;

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Shield")
	FOnShieldBreak OnShieldBreak;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	float CooldownShieldRecoverTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	float ShieldRecoverValue = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	float ShieldRecoverRate = 0.1f;

protected:

	float Shield = 100.0f;

public:

	void ChangeHealthValue(float ChangeValue) override;

	float GetCurrentShield();

	void ChangeShieldValue(float ChangeValue);

	void CooldownShieldEnd();

	void RecoveryShield();

	UFUNCTION(BlueprintCallable)
	float GetShieldValue();
	
};
