// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"

#include "FuncLibrary/UserTypes.h"
#include "Weapons/WeaponDefault.h"

#include "PVXGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVORTEX_API UPVXGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// Table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Setting")
	UDataTable* WeaponInfoTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Setting")
	UDataTable* DropItemInfoTable = nullptr;

	UFUNCTION(BlueprintCallable)
	bool GetWeaponInfoByName(FName NameWeapon, FWeaponInfo& OutInfo);

	UFUNCTION(BlueprintCallable)
	bool GetDropItemInfoByWeaponName(FName NameItem, FDropItem& OutInfo);
	
};
