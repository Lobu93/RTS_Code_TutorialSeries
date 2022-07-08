// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_VehiclePawn.h"
#include "TP_VehicleWheelFront.h"
#include "TP_VehicleWheelRear.h"
#include "Components/SkeletalMeshComponent.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "GameFramework/Controller.h"
#include "Components/DecalComponent.h"
#include "../Player/RTS_PlayerController.h"
#include "../RTS_Cpp_TutorialCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

ATP_VehiclePawn::ATP_VehiclePawn()
{
	// Car mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("/Game/Vehicle/Sedan/Sedan_SkelMesh.Sedan_SkelMesh"));
	GetMesh()->SetSkeletalMesh(CarMesh.Object);

	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/Vehicle/Sedan/Sedan_AnimBP"));
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);
	
	// Simulation
	Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	check(Vehicle4W->WheelSetups.Num() == 4);

	Vehicle4W->WheelSetups[0].WheelClass = UTP_VehicleWheelFront::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("Wheel_Front_Left");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[1].WheelClass = UTP_VehicleWheelFront::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("Wheel_Front_Right");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	Vehicle4W->WheelSetups[2].WheelClass = UTP_VehicleWheelRear::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("Wheel_Rear_Left");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[3].WheelClass = UTP_VehicleWheelRear::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("Wheel_Rear_Right");
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	bInReverseGear = false;

	// Create a decal in the world to show the cursor's location
	SelectedDecal = CreateDefaultSubobject<UDecalComponent>("SelectedDecal");
	SelectedDecal->SetupAttachment(RootComponent);
	SelectedDecal->DecalSize = FVector(16.0f, 120.0f, 120.0f);
	SelectedDecal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	SelectedDecal->SetVisibility(false);
	SelectedDecal->bHiddenInGame = true;

	// Create a decal in the world to show the unselected characters
	DeselectedDecal = CreateDefaultSubobject<UDecalComponent>("DeselectedDecal");
	DeselectedDecal->SetupAttachment(RootComponent);
	DeselectedDecal->DecalSize = FVector(16.0f, 120.0f, 120.0f);
	DeselectedDecal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
}

void ATP_VehiclePawn::Tick(float Delta)
{
	Super::Tick(Delta);

	// Setup the flag to say we are in reverse gear
	bInReverseGear = GetVehicleMovement()->GetCurrentGear() < 0;
}

void ATP_VehiclePawn::BeginPlay()
{
	Super::BeginPlay();

	ControllerRef = Cast<ARTS_PlayerController>(GetWorld()->GetFirstPlayerController());
}

void ATP_VehiclePawn::SetSelectedDecal()
{
	DeselectedDecal->SetVisibility(false);
	DeselectedDecal->SetHiddenInGame(true);
	SelectedDecal->SetVisibility(true);
	SelectedDecal->SetHiddenInGame(false);
}

void ATP_VehiclePawn::SetDeselectedDecal()
{
	SelectedDecal->SetVisibility(false);
	SelectedDecal->SetHiddenInGame(true);
	DeselectedDecal->SetVisibility(true);
	DeselectedDecal->SetHiddenInGame(false);
}

void ATP_VehiclePawn::MoveToLocation(FVector TargetLocation)
{
	Target = TargetLocation;

	InitialDistance = UKismetMathLibrary::VSize(GetActorLocation() - Target);

	StartingTime = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());

	Vehicle4W->SetHandbrakeInput(false);

	GetWorldTimerManager().SetTimer(TimerHandleGlobal, this, &ATP_VehiclePawn::UpdateMovement, 0.01f, true);
}

void ATP_VehiclePawn::UpdateMovement()
{
	FVector CurrentLocationLocal;
	float CurrentDistanceLocal = 0.0f;
	float CurrentInputLocal;

	FRotator LookAtRotationLocal;
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
	LookAtRotationLocal = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	DeltaRotatorLocal = UKismetMathLibrary::NormalizedDeltaRotator(LookAtRotationLocal, GetActorRotation());
	MapRangeLocal = FMath::GetMappedRangeValueUnclamped(InputRange, OutputRange, DeltaRotatorLocal.Yaw);
	bIsInRangeLocal = UKismetMathLibrary::InRange_FloatFloat(DeltaRotatorLocal.Yaw, -90.0f, 90.0f);
	FirstSelectFloatLocal = UKismetMathLibrary::SelectFloat(MapRangeLocal, 0.0f, bIsInRangeLocal);
	bSecondPickALocal = (!bIsInRangeLocal && CurrentDistanceLocal < 1000.0f) ? true : false;
	SecondSelectFloatLocal = UKismetMathLibrary::SelectFloat(FirstSelectFloatLocal, MapRangeLocal, bSecondPickALocal);
	bThirdPickALocal = (FirstSelectFloatLocal == 0.0f) ? true : false;
	SteeringLocal = UKismetMathLibrary::SelectFloat(-1.0f, SecondSelectFloatLocal, bThirdPickALocal);
	Vehicle4W->SetSteeringInput(SteeringLocal);

	// VectorLenght = (CurrentLocationLocal - Target).Size();
	CurrentDistanceLocal = UKismetMathLibrary::VSize(CurrentLocationLocal - Target);

	// Throttle Calculation
	ThrottleValue_B = UKismetMathLibrary::SelectFloat(1.0f, -1.0f, bIsInRangeLocal);
	float BaseLocal = InitialDistance * 0.000025f + 1.0f;
	// float ExpTimeLocal = GetWorld()->GetTimeSeconds() - StartingTime;
	float ExpTimeLocal = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld()) - StartingTime;
	PoweredDistanceLocal = UKismetMathLibrary::MultiplyMultiply_FloatFloat(BaseLocal, ExpTimeLocal);
	CurrentDistanceDivByInitialLocal = CurrentDistanceLocal / InitialDistance;
	ThrottleValue_A = (CurrentDistanceDivByInitialLocal * PoweredDistanceLocal) * ThrottleValue_B;
	bThrottlePickALocal = (CurrentDistanceDivByInitialLocal < 5000.0f) ? true : false;
	ThrottleValue_B *= 0.5f;
	ThrottleSelectedFloatLocal = UKismetMathLibrary::SelectFloat(ThrottleValue_A, ThrottleValue_B, bThrottlePickALocal);
	CurrentInputLocal = FMath::Clamp(ThrottleSelectedFloatLocal, -10.0f, 10.0f);
	Vehicle4W->SetThrottleInput(CurrentInputLocal);
	
	// Brake Section
	if ((CurrentDistanceLocal / InitialDistance) <= 0.5f)
	{
		Vehicle4W->SetBrakeInput(FMath::Abs(CurrentInputLocal * 1.7f));

		if ((CurrentDistanceLocal / InitialDistance) <= 0.25f)
		{
			Vehicle4W->SetHandbrakeInput(true);
			Vehicle4W->SetThrottleInput(0.0f);
			Vehicle4W->SetBrakeInput(0.0f);
			GetWorldTimerManager().ClearTimer(TimerHandleGlobal);
			ControllerRef->RemoveLocationDecals();

			if (bHasPassengers)
			{
				RemovePassengers();
			}
		}
	}
}

void ATP_VehiclePawn::ReceiveMoveCommand(FVector TargetLocation)
{
	MoveToLocation(TargetLocation);
}

void ATP_VehiclePawn::GetPassengers(TArray<ARTS_Cpp_TutorialCharacter*> Passengers)
{
	PassengersTemp = Passengers;

	for (auto Unit : PassengersTemp)
	{
		if (Unit->PassengerIn == this)
		{
			CurrentPassengers.AddUnique(Unit);
		}
	}
}

void ATP_VehiclePawn::RemovePassengers()
{
	ARTS_Cpp_TutorialCharacter* UnitPassengerLocal;
	FVector UnitLocationLocal;
	FVector DestLocationLocal;
	float RandomFloatLocal;

	for (auto Unit : CurrentPassengers)
	{
		UnitPassengerLocal = Unit;
		UnitPassengerLocal->SetActorHiddenInGame(false);
		
		UnitLocationLocal = GetActorLocation();
		RandomFloatLocal = FMath::RandRange(200.0f, 500.0f);
		
		DestLocationLocal.X = UnitLocationLocal.X + RandomFloatLocal;
		DestLocationLocal.Y = UnitLocationLocal.Y + RandomFloatLocal;
		DestLocationLocal.Z = UnitLocationLocal.Z + 150.0f;

		UnitPassengerLocal->TeleportTo(DestLocationLocal, FRotator(0.0f, 0.0f, 0.0f));

		UnitPassengerLocal->ExitVehicle();
	}

	CurrentPassengers.Empty();

	bHasPassengers = false;
}


PRAGMA_ENABLE_DEPRECATION_WARNINGS
