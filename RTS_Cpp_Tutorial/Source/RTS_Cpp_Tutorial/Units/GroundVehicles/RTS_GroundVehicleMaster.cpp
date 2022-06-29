// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS_GroundVehicleMaster.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Components/DecalComponent.h"
//#include "../../TP_Vehicle/TP_VehicleWheelFront.h"
//#include "../../TP_Vehicle/TP_VehicleWheelRear.h"
// #include "WheeledVehicleMovementComponent4W.h"

ARTS_GroundVehicleMaster::ARTS_GroundVehicleMaster()
{
	UChaosWheeledVehicleMovementComponent* VehicleMovComp = CastChecked<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());
	//Torque setup
	VehicleMovComp->EngineSetup.MaxRPM = 5700.f;
	VehicleMovComp->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	VehicleMovComp->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.f, 400.f);
	VehicleMovComp->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(2000.f, 500.f);
	VehicleMovComp->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5800, 400);

	//Adjust The steering
	VehicleMovComp->SteeringSetup.SteeringCurve.GetRichCurve()->Reset();
	VehicleMovComp->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(0.0f, 1.0f);
	VehicleMovComp->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(40.0f, 0.7f);
	VehicleMovComp->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(120.0f, 0.6f);

	//Differential Setup
	VehicleMovComp->DifferentialSetup.DifferentialType = EVehicleDifferential::AllWheelDrive;
	VehicleMovComp->DifferentialSetup.FrontRearSplit = 0.65;
	
	//Automatic Gear Box Setup
	VehicleMovComp->TransmissionSetup.bUseAutomaticGears = true;
	VehicleMovComp->TransmissionSetup.GearChangeTime = 0.15f;
	VehicleMovComp->TransmissionSetup.bUseAutoReverse = true;

	// Create a decal in the world to show the cursor's location
	SelectedDecal = CreateDefaultSubobject<UDecalComponent>("SelectedDecal");
	SelectedDecal->SetupAttachment(RootComponent);
	SelectedDecal->DecalSize = FVector(16.0f, 120.0f, 120.0f);
	SelectedDecal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	//SelectedDecal->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	SelectedDecal->SetVisibility(false);
	SelectedDecal->bHiddenInGame = true;

	// Create a decal in the world to show the unselected characters
	DeselectedDecal = CreateDefaultSubobject<UDecalComponent>("DeselectedDecal");
	DeselectedDecal->SetupAttachment(RootComponent);
	DeselectedDecal->DecalSize = FVector(16.0f, 120.0f, 120.0f);
	DeselectedDecal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	//DeselectedDecal->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
}

//void ARTS_GroundVehicleMaster::BeginPlay()
//{
//	Super::BeginPlay();
//}

void ARTS_GroundVehicleMaster::SetSelectedDecal()
{
	DeselectedDecal->SetVisibility(false);
	DeselectedDecal->SetHiddenInGame(true);
	SelectedDecal->SetVisibility(true);
	SelectedDecal->SetHiddenInGame(false);
}

void ARTS_GroundVehicleMaster::SetDeselectedDecal()
{
	SelectedDecal->SetVisibility(false);
	SelectedDecal->SetHiddenInGame(true);
	DeselectedDecal->SetVisibility(true);
	DeselectedDecal->SetHiddenInGame(false);
}
