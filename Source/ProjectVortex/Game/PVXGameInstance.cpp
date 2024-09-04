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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UPVXGameInstance::GetWeaponInfoByName - WeaponInfoTable -NULL"));
	}

	return bIsFound;
}

bool UPVXGameInstance::GetDropItemInfoByWeaponName(FName NameItem, FDropItem& OutInfo)
{
	bool bIsFind = false;

	if (DropItemInfoTable)
	{
		FDropItem* DropItemInfoRow;
		TArray<FName> RowNames = DropItemInfoTable->GetRowNames();

		int8 i = 0;
		while (i < RowNames.Num() && !bIsFind)
		{
			DropItemInfoRow = DropItemInfoTable->FindRow<FDropItem>(RowNames[i], "");
			if (DropItemInfoRow->WeaponInfo.NameItem == NameItem)
			{
				OutInfo = *DropItemInfoRow;
				bIsFind = true;
			}
			++i;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UPVXGameInstance::GetDropItemInfoByName - DropItemInfoTable -NULL"));
	}
	return bIsFind;
}
