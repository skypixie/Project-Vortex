// Fill out your copyright notice in the Description page of Project Settings.


#include "FuncLibrary/UserTypes.h"
#include "ProjectVortex.h"
#include "Interface/UGameActor.h"


void UUserTypes::AddEffectSurfaceType(AActor* TakeEffectActor, TSubclassOf<UStateEffect> AddEffectClass, EPhysicalSurface SurfaceType)
{
	/*if (SurfaceType != EPhysicalSurface::SurfaceType_Default && TakeEffectActor && AddEffectClass)
	{
		UStateEffect* myEffect = Cast<UStateEffect>(AddEffectClass->GetDefaultObject());
		if (myEffect)
		{
			bool bIsHavePossibleSurface = false;
			int8 i = 0;
			while (i < myEffect->PossibleInteractSurface.Num() && !bIsHavePossibleSurface)
			{
				if (myEffect->PossibleInteractSurface[i] == SurfaceType)
				{
					bIsHavePossibleSurface = true;
					bool bIsCanAddEffect = false;
					if (!myEffect->bIsStackable)
					{
						int8 j = 0;
						TArray<UStateEffect*> CurrentEffects;
						IUGameActor* myInterface = Cast<IUGameActor>(TakeEffectActor);
						if (myInterface)
						{
							CurrentEffects = myInterface->GetAllCurrentEffects();
						}

						if (CurrentEffects.Num() > 0)
						{
							while (j < CurrentEffects.Num() && !bIsCanAddEffect)
							{
								if (CurrentEffects[j]->GetClass() != AddEffectClass)
								{
									bIsCanAddEffect = true;
								}
								j++;
							}
						}
						else
						{
							bIsCanAddEffect = true;
						}

					}
					else
					{
						bIsCanAddEffect = true;
					}

					if (bIsCanAddEffect)
					{

						UStateEffect* NewEffect = NewObject<UStateEffect>(TakeEffectActor, AddEffectClass);
						if (NewEffect)
						{
							NewEffect->InitObject(TakeEffectActor);
						}
					}

				}
				i++;
			}
		}

	}*/

	if (SurfaceType != EPhysicalSurface::SurfaceType_Default && TakeEffectActor && AddEffectClass)
	{
		UStateEffect* myEffect = Cast<UStateEffect>(AddEffectClass.GetDefaultObject());
		if (myEffect)
		{
			bool bHasPossibleSurface = false;
			int8 i = 0;
			while (i < myEffect->PossibleInteractSurface.Num() && !bHasPossibleSurface)
			{
				if (myEffect->PossibleInteractSurface[i] == SurfaceType)
				{
					bHasPossibleSurface = true;
					bool bCanAddEffect = false;
					if (!myEffect->bIsStackable)
					{
						int8 j = 0;
						TArray<UStateEffect*> CurrentEffects;
						IUGameActor* myInterface = Cast<IUGameActor>(TakeEffectActor);
						if (myInterface)
						{
							CurrentEffects = myInterface->GetAllCurrentEffects();
						}

						if (CurrentEffects.Num() > 0)
						{
							while (j < CurrentEffects.Num() && !bCanAddEffect)
							{
								if (CurrentEffects[j]->GetClass() != AddEffectClass)
								{
									bCanAddEffect = true;
								}
								++j;
							}
						}
						else
						{
							bCanAddEffect = true;
						}
					}

					else
					{
						bCanAddEffect = true;
					}

					if (bCanAddEffect)
					{
						UStateEffect* NewEffect = NewObject<UStateEffect>(TakeEffectActor, AddEffectClass);
						if (NewEffect)
						{
							NewEffect->InitObject(TakeEffectActor);
						}
					}
				}
				++i;
			}
		}
	}
}
