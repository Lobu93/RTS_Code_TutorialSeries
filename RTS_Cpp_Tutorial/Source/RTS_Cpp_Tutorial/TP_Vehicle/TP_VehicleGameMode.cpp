// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_VehicleGameMode.h"
#include "TP_VehiclePawn.h"

ATP_VehicleGameMode::ATP_VehicleGameMode()
{
	DefaultPawnClass = ATP_VehiclePawn::StaticClass();
}
