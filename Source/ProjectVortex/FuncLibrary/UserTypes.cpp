// Fill out your copyright notice in the Description page of Project Settings.


#include "FuncLibrary/UserTypes.h"
#include "ProjectVortex.h"

void UUserTypes::AddEffectSurfaceType(AActor* TakeEffectActor, TSubclassOf<UStateEffect> AddEffectClass, EPhysicalSurface SurfaceType)
{
	if (SurfaceType != EPhysicalSurface::SurfaceType_Default && TakeEffectActor && AddEffectClass)
	{
		UStateEffect* myEffect = Cast<UStateEffect>(AddEffectClass.GetDefaultObject());
		if (myEffect)
		{
			bool bCanAdd = false;
			int8 i = 0;
			while (i < myEffect->PossibleInteractSurface.Num() && !bCanAdd)
			{
				if (myEffect->PossibleInteractSurface[i] == SurfaceType)
				{
					bCanAdd = true;
					UStateEffect* NewEffect = NewObject<UStateEffect>(TakeEffectActor, FName("Effect"));

					if (NewEffect)
					{
						NewEffect->InitObject(TakeEffectActor);
					}
				}
				++i;
			}
		}
	}
}
