// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS_PlayerController.h"
#include "RTS_CameraPawn.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
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

	bShowMouseCursor = true;
}

void ARTS_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ARTS_PlayerController::Move_CameraPawn_X);
	InputComponent->BindAxis("MoveRight", this, &ARTS_PlayerController::Move_CameraPawn_Y);

	InputComponent->BindAction("ZoomReset", IE_Pressed, this, &ARTS_PlayerController::ZoomReset);
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ARTS_PlayerController::ZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &ARTS_PlayerController::ZoomOut);

	InputComponent->BindAxis("Mouse_X", this, &ARTS_PlayerController::MousePan_X);
	InputComponent->BindAxis("Mouse_Y", this, &ARTS_PlayerController::MousePan_Y);

	InputComponent->BindAction("Pan", IE_Pressed, this, &ARTS_PlayerController::PanPressed);
	InputComponent->BindAction("Pan", IE_Released, this, &ARTS_PlayerController::PanReleased);
	InputComponent->BindAction("PanReset", IE_Pressed, this, &ARTS_PlayerController::PanReset);

	InputComponent->BindAction("SpeedModifier", IE_Pressed, this, &ARTS_PlayerController::SpeedModifierPressed);
	InputComponent->BindAction("SpeedModifier", IE_Released, this, &ARTS_PlayerController::SpeedModifierReleased);
}

void ARTS_PlayerController::Move_CameraPawn_X(float AxisValue)
{
	float X_DirectionLocal;
	FVector DirectionLocal;
	FTransform ActorTransformLocal;
	FVector LocationLocal;
	FVector ActorNewLocation;
	
	X_DirectionLocal = (AxisValue * DefaultMovementSpeed) * MovementSpeedModifier;

	DirectionLocal = FVector(X_DirectionLocal, 0.0f, 0.0f);

	ActorTransformLocal = CameraPawnRef->GetActorTransform();

	LocationLocal = UKismetMathLibrary::TransformDirection(ActorTransformLocal, DirectionLocal);

	LocationLocal += ActorTransformLocal.GetLocation();

	ActorTransformLocal = FTransform(ActorTransformLocal.GetRotation(), LocationLocal, ActorTransformLocal.GetScale3D());

	ActorNewLocation = FVector(ActorTransformLocal.GetLocation().X, ActorTransformLocal.GetLocation().Y, 200.0f);

	if (!bDisableCamMovement)
	{
		CameraPawnRef->SetActorLocation(ActorNewLocation);
	}
}

void ARTS_PlayerController::Move_CameraPawn_Y(float AxisValue)
{
	float Y_DirectionLocal;
	FVector DirectionLocal;
	FTransform ActorTransformLocal;
	FVector ActorNewLocation;

	Y_DirectionLocal = AxisValue * (DefaultMovementSpeed * MovementSpeedModifier);

	DirectionLocal = FVector(0.0f, Y_DirectionLocal , 0.0f);

	ActorTransformLocal = CameraPawnRef->GetActorTransform();

	ActorNewLocation = UKismetMathLibrary::TransformDirection(ActorTransformLocal, DirectionLocal);

	ActorNewLocation += ActorTransformLocal.GetLocation();

	if (!bDisableCamMovement)
	{
		CameraPawnRef->SetActorLocation(ActorNewLocation);
	}
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

void ARTS_PlayerController::PanPressed()
{
	bDisableCamMovement = true;
}

void ARTS_PlayerController::PanReleased()
{
	bDisableCamMovement = false;
}

void ARTS_PlayerController::PanReset()
{
	FRotator NewRotationLocal;
	FRotator ActorRotationLocal;

	ActorRotationLocal = CameraPawnRef->GetActorRotation();

	NewRotationLocal = FRotator(0.0f, ActorRotationLocal.Yaw, ActorRotationLocal.Roll);

    CameraPawnRef->SetActorRotation(NewRotationLocal);

}

void ARTS_PlayerController::MousePan_X(float AxisValue)
{
	FRotator NewRotationLocal;
	FRotator ActorRotationLocal;
	float YawLocal;

	ActorRotationLocal = CameraPawnRef->GetActorRotation();

	YawLocal = ActorRotationLocal.Yaw + (AxisValue * PanSpeed);

	NewRotationLocal = FRotator(ActorRotationLocal.Pitch, YawLocal, ActorRotationLocal.Roll);

	CameraPawnRef->SetActorRotation(NewRotationLocal);
}

void ARTS_PlayerController::MousePan_Y(float AxisValue)
{
	FRotator NewRotationLocal;
	FRotator ActorRotationLocal;
	float PitchLocal;

	ActorRotationLocal = CameraPawnRef->GetActorRotation();

	PitchLocal = ActorRotationLocal.Pitch + (AxisValue * PanSpeed);

	PitchLocal = FMath::Clamp(PitchLocal, -25.0f, 45.0f);

	NewRotationLocal = FRotator(PitchLocal, ActorRotationLocal.Yaw, ActorRotationLocal.Roll);

	CameraPawnRef->SetActorRotation(NewRotationLocal);
}

void ARTS_PlayerController::SpeedModifierPressed()
{
	MovementSpeedModifier = 2.0f;
}

void ARTS_PlayerController::SpeedModifierReleased()
{
	MovementSpeedModifier = 1.0f;
}
