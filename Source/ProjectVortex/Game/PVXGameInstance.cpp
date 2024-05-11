// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PVXGameInstance.h"

bool UPVXGameInstance::GetWeaponInfoByName(FName NameWeapon, FWeaponInfo& OutInfo)
{
	bool bIsFound = false;
	FWeaponInfo* WeaponInfoRow;

	if (WeaponInfoTable)
	{
		WeaponInfoRow = WeaponInfoTable->FindRow<FWeaponInfo>(NameWeapon, "", false);
		if (WeaponInfoRow)
		{
			bIsFound = true;
			OutInfo = *WeaponInfoRow;
		}
	}
	return bIsFound;
}
