// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS_GroundVehicleMaster.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Components/DecalComponent.h"
#include "RTS_ChaosVehicleWheelFront.h"
#include "RTS_ChaosVehicleWheelRear.h"
#include "../../Player/RTS_PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
//#include "../../TP_Vehicle/TP_VehicleWheelFront.h"
//#include "../../TP_Vehicle/TP_VehicleWheelRear.h"
// #include "WheeledVehicleMovementComponent4W.h"

ARTS_GroundVehicleMaster::ARTS_GroundVehicleMaster()
{
	VehicleMovComp = CastChecked<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());
	
	// Initialize WheelSetups array with 4 wheels
	VehicleMovComp->WheelSetups.SetNum(4);

	// Wheels/Tyres
	// Setup the wheels
	VehicleMovComp->WheelSetups[0].WheelClass = URTS_ChaosVehicleWheelFront::StaticClass();
	VehicleMovComp->WheelSetups[0].BoneName = FName("Wheel_Front_Left");
	VehicleMovComp->WheelSetups[0].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	VehicleMovComp->WheelSetups[1].WheelClass = URTS_ChaosVehicleWheelFront::StaticClass();
	VehicleMovComp->WheelSetups[1].BoneName = FName("Wheel_Front_Right");
	VehicleMovComp->WheelSetups[1].AdditionalOffset = FVector(0.f, 12.f, 0.f);
	
	VehicleMovComp->WheelSetups[2].WheelClass = URTS_ChaosVehicleWheelRear::StaticClass();
	VehicleMovComp->WheelSetups[2].BoneName = FName("Wheel_Rear_Left");
	VehicleMovComp->WheelSetups[2].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	VehicleMovComp->WheelSetups[3].WheelClass = URTS_ChaosVehicleWheelRear::StaticClass();
	VehicleMovComp->WheelSetups[3].BoneName = FName("Wheel_Rear_Right");
	VehicleMovComp->WheelSetups[3].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	//Engine - Torque setup
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

	//Differential Setup - Transmission
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

void ARTS_GroundVehicleMaster::BeginPlay()
{
	Super::BeginPlay();

	ControllerRef = Cast<ARTS_PlayerController>(GetWorld()->GetFirstPlayerController());
}

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

void ARTS_GroundVehicleMaster::MoveToLocation(FVector TargetLocation)
{
	FVector DistanceLocal;
	FTimerHandle TimerHandleLocal;

	Target = TargetLocation;

	DistanceLocal = GetActorLocation() - Target;

	InitialDistance = DistanceLocal.Size();

	StartingTime = GetWorld()->GetTimeSeconds();

	VehicleMovComp->SetHandbrakeInput(false);

	GetWorldTimerManager().SetTimer(TimerHandleLocal, this, &ARTS_GroundVehicleMaster::UpdateMovement, 0.01f, true);
}

void ARTS_GroundVehicleMaster::UpdateMovement()
{
	FVector CurrentLocationLocal;
	float CurrentDistanceLocal = 0.0f;
	float CurrentInputLocal;
	
	FRotator DeltaRotatorLocal;
	float MapRangeLocal;
	bool bIsInRangeLocal;
	bool bSecondPickALocal;
	bool bThirdPickALocal;
	FVector2D InputRange = FVector2D(-180.0f, 180.0f);
	FVector2D OutputRange = FVector2D(-1.0f, 1.0f);
	float FirstSelectFloatLocal;
	float SecondSelectFloatLocal;
	float SteeringLocal;

	float PoweredDistanceLocal;
	float ThrottleValue_A;
	float ThrottleValue_B;
	float CurrentDistanceDivByInitialLocal;
	bool bThrottlePickALocal;
	float ThrottleSelectedFloatLocal;

	CurrentLocationLocal = GetActorLocation();

	// Steering Calculation
	DeltaRotatorLocal = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	MapRangeLocal = FMath::GetMappedRangeValueUnclamped(InputRange, OutputRange, DeltaRotatorLocal.Yaw);
	bIsInRangeLocal = UKismetMathLibrary::InRange_FloatFloat(DeltaRotatorLocal.Yaw, -90.0f, 90.0f);
	FirstSelectFloatLocal = UKismetMathLibrary::SelectFloat(MapRangeLocal, 0.0f, bIsInRangeLocal);
	bSecondPickALocal = (!bIsInRangeLocal && CurrentDistanceLocal < 1000.0f) ? true : false;
	SecondSelectFloatLocal = UKismetMathLibrary::SelectFloat(FirstSelectFloatLocal, MapRangeLocal, bSecondPickALocal);
	bThirdPickALocal = (FirstSelectFloatLocal == 0.0f) ? true : false;
	SteeringLocal = UKismetMathLibrary::SelectFloat(-1.0f, SecondSelectFloatLocal, bThirdPickALocal);
	VehicleMovComp->SetSteeringInput(SteeringLocal);

	CurrentDistanceLocal = (CurrentLocationLocal - Target).Size();
	
	// Throttle Calculation
	ThrottleValue_B = UKismetMathLibrary::SelectFloat(1.0f, -1.0f, bIsInRangeLocal);
	PoweredDistanceLocal = powf((InitialDistance * 0.000025f + 1.0f), GetWorld()->GetTimeSeconds() - StartingTime);
	CurrentDistanceDivByInitialLocal = CurrentDistanceLocal / InitialDistance;
	ThrottleValue_A = (CurrentDistanceDivByInitialLocal * PoweredDistanceLocal) * ThrottleValue_B;
	bThrottlePickALocal = (CurrentDistanceDivByInitialLocal < 5000.0f) ? true : false;
	ThrottleValue_B *= 0.5f;
	ThrottleSelectedFloatLocal = UKismetMathLibrary::SelectFloat(ThrottleValue_A, ThrottleValue_B, bThirdPickALocal);
	CurrentInputLocal = FMath::Clamp(ThrottleSelectedFloatLocal, -10.0f, 10.0f);
	VehicleMovComp->SetThrottleInput(CurrentInputLocal);

	UE_LOG(LogTemp, Warning, TEXT("ARTS_GroundVehicleMaster::UpdateMovement() | CurrentInputLocal: %f"),
		CurrentInputLocal);
}

void ARTS_GroundVehicleMaster::ReceiveMoveCommand(FVector TargetLocation)
{
	MoveToLocation(TargetLocation);
}
