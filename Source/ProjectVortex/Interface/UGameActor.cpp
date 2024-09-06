// Fill out your copyright notice in the Description page of Project Settings.


#include "UGameActor.h"

// Add default functionality here for any IUGameActor functions that are not pure virtual.

EPhysicalSurface IUGameActor::GetSurfaceType()
{
    return EPhysicalSurface::SurfaceType_Default;
}
