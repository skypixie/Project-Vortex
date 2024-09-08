// Fill out your copyright notice in the Description page of Project Settings.


#include "UGameActor.h"

#include "StateEffects/StateEffect.h"

// Add default functionality here for any IUGameActor functions that are not pure virtual.

EPhysicalSurface IUGameActor::GetSurfaceType()
{
    return EPhysicalSurface::SurfaceType_Default;
}

TArray<UStateEffect*> IUGameActor::GetAllCurrentEffects()
{
    return Effects;
}

void IUGameActor::RemoveEffect(UStateEffect* Effect)
{
    Effects.Remove(Effect);
}

void IUGameActor::AddEffect(UStateEffect* NewEffect)
{
    Effects.Add(NewEffect);
}
