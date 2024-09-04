// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectVortexGameMode.generated.h"

UCLASS(minimalapi)
class AProjectVortexGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectVortexGameMode();

	void PlayerCharacterDead();
};



