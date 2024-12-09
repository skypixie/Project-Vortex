// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterHealthComponent.h"

void UCharacterHealthComponent::ChangeHealthValue(float ChangeValue)
{
	float CurrentDamage = ChangeValue * CoefDamage;

	if (Shield > 0.0f && ChangeValue < 0.0f)
	{
		ChangeShieldValue(ChangeValue);

		if (Shield <= 0.0f)
		{
			OnShieldBreak.Broadcast();
		}
	}
	else
	{
		Super::ChangeHealthValue(ChangeValue);
	}

}

float UCharacterHealthComponent::GetCurrentShield()
{
	return Shield;
}

void UCharacterHealthComponent::ChangeShieldValue(float ChangeValue)
{
	Shield += ChangeValue;

	OnShieldChange.Broadcast(Shield, ChangeValue);

	if (Shield > 100.0f)
	{
		Shield = 100.0f;
	}
	else
	{
		if (Shield < 0.0f)
		{
			Shield = 0.0f;
		}
	}

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_CooldownShieldTimer, this, &UCharacterHealthComponent::CooldownShieldEnd, CooldownShieldRecoverTime, false);

		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ShieldRecoveryRateTimer);
	}
}

void UCharacterHealthComponent::CooldownShieldEnd()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ShieldRecoveryRateTimer, this, &UCharacterHealthComponent::RecoveryShield, ShieldRecoverRate, true);

	}
}

void UCharacterHealthComponent::RecoveryShield()
{
	float tmp = Shield;
	tmp = tmp + ShieldRecoverValue;
	if (tmp > 100.0f)
	{
		Shield = 100.f;
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ShieldRecoveryRateTimer);	
		}
	}
	else
	{
		Shield = tmp;
	}

	OnShieldChange.Broadcast(Shield, ShieldRecoverValue);
}

float UCharacterHealthComponent::GetShieldValue()
{
	return Shield;
}
