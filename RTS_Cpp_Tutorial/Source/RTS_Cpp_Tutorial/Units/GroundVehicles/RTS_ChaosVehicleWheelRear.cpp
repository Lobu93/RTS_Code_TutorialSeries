// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS_ChaosVehicleWheelRear.h"

URTS_ChaosVehicleWheelRear::URTS_ChaosVehicleWheelRear()
{
	WheelRadius = 35.f;
	WheelWidth = 10.0f;
	bAffectedByHandbrake = true;
	// bAffectedByEngine = true;
	MaxSteerAngle = 0.f;
	bAffectedBySteering = false;
}

