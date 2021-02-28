// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RTS_PlayerController.generated.h"

class ARTS_CameraPawn;

/**
 * 
 */
UCLASS()
class RTS_CPP_TUTORIAL_API ARTS_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Begin PlayerController interface
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	void Move_CameraPawn_X(float AxisValue);
	
	void Move_CameraPawn_Y(float AxisValue);

	void ZoomReset();

	void ZoomIn();

	void ZoomOut();

	void PanPressed();
	
	void PanReleased();

	void PanReset();

	void MousePan_X(float AxisValue);

	void MousePan_Y(float AxisValue);

private:
	// How close can the camera get to the root
	float MinZoomLimit = 300.0f;

	// How far can the camera move away from the root
	float MaxZoomLimit = 4000.0f;

	// Determines if certain key has been pressed
	bool bIsPressed;

	// When mouse is pressed, the camera don't can move
	bool bDisableCamMovement;

public:
	// This is a reference to our camera Pawn
	ARTS_CameraPawn* CameraPawnRef;

	// How fast the camera moves when zooming
	float ZoomSpeed = 150.0f;

	// How fast the camera can pans
	float PanSpeed = 5.0f;

};
