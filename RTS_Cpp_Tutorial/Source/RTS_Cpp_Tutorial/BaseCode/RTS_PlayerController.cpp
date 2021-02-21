// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS_PlayerController.h"
#include "RTS_CameraPawn.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
// #include "Engine/World.h"

// Called when the game starts or when spawned
void ARTS_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Cast references once for the entire code, reduce system drain.
	CameraPawnRef = (ARTS_CameraPawn*)UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!CameraPawnRef)
	{
		UE_LOG(LogTemp, Error, TEXT("ARTS_PlayerController::BeginPlay() Bad CameraPawn Class"));
	}
}

void ARTS_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ARTS_PlayerController::Move_CameraPawn_X);
	InputComponent->BindAxis("MoveRight", this, &ARTS_PlayerController::Move_CameraPawn_Y);

	InputComponent->BindAction("ZoomReset", IE_Pressed, this, &ARTS_PlayerController::ZoomReset);
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ARTS_PlayerController::ZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &ARTS_PlayerController::ZoomOut);
}

void ARTS_PlayerController::Move_CameraPawn_X(float AxisValue)
{
	FVector AxisDirectionLocal = FVector(AxisValue, 0.0f, 0.0f);

	CameraPawnRef->FloatingPawnMovement->AddInputVector(AxisDirectionLocal);
}

void ARTS_PlayerController::Move_CameraPawn_Y(float AxisValue)
{
	FVector AxisDirectionLocal = FVector(0.0f, AxisValue, 0.0f);

	CameraPawnRef->FloatingPawnMovement->AddInputVector(AxisDirectionLocal);
}

void ARTS_PlayerController::ZoomReset()
{
	CameraPawnRef->SpringArm->TargetArmLength = 1500.0f;
}

void ARTS_PlayerController::ZoomIn()
{
	float TargetArmLengthLocal;

	TargetArmLengthLocal = CameraPawnRef->SpringArm->TargetArmLength - ZoomSpeed;

	CameraPawnRef->SpringArm->TargetArmLength = FMath::Clamp(TargetArmLengthLocal, MinZoomLimit, MaxZoomLimit);
}

void ARTS_PlayerController::ZoomOut()
{
	float TargetArmLengthLocal;

	TargetArmLengthLocal = CameraPawnRef->SpringArm->TargetArmLength + ZoomSpeed;

	CameraPawnRef->SpringArm->TargetArmLength = FMath::Clamp(TargetArmLengthLocal, MinZoomLimit, MaxZoomLimit);
}
