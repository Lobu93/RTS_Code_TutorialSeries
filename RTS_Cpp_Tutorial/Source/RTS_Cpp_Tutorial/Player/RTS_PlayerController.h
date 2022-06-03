// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RTS_PlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDisplayUnitHUD, class AActor*, HitActor, bool, bBypass);

class ARTS_CameraPawn;
class ARTS_GameState;
class ARTS_Cpp_TutorialCharacter;

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

	void SpeedModifierPressed();

	void SpeedModifierReleased();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	FVector EdgeScroll();

	void SecondaryAction();

	// Cast references once for the entire code, reduce system drain.
	void ReferenceCasts();

private:
	// How close can the camera get to the root
	float MinZoomLimit = 300.0f;

	// How far can the camera move away from the root
	float MaxZoomLimit = 4000.0f;

	float EdgeScrollSpeedX;

	float EdgeScrollSpeedY;

public:
	// This is a reference to our camera Pawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	ARTS_CameraPawn* CameraPawnRef;

	ARTS_GameState* GameStateRef;

	// DisplayUnit HUD Event Dispatcher
	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FDisplayUnitHUD DisplayUnitHUD_Delegate;

	const TArray<TEnumAsByte<EObjectTypeQuery>> SelectableObjectsEnum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
	TSubclassOf<ARTS_Cpp_TutorialCharacter> UnitForDebug;

	// How fast the camera moves when zooming
	float ZoomSpeed = 150.0f;

	// How fast the camera can pans
	float PanSpeed = 5.0f;

	// How fast the camera moves
	float DefaultMovementSpeed = 15.0f;

	// Modify default movement speed
	float MovementSpeedModifier = 1.0f;

	// When mouse is pressed, the camera don't can move
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bDisableCamMovement;

	FVector2D MouseLastValidPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Resources")
	int32 WoodResource = 1550;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Resources")
	int32 OreResource = 1200;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Resources")
	int32 MaxResourceLimit;

	bool bIsUnitSelected;

	void SpawnUnitDebug();

};
