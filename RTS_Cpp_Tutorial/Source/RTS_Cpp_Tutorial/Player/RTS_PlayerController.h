// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../Library/RTS_MarqueeSelection_IF.h"
#include "RTS_PlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDisplayUnitHUD, class AActor*, HitActor, bool, bBypass);

class ARTS_CameraPawn;
class ARTS_GameState;
class ARTS_Cpp_TutorialCharacter;
class ATP_VehiclePawn;
class ARTS_MarqueeSelection;
class UDecalComponent;
class AAIController;

/**
 * 
 */
UCLASS()
class RTS_CPP_TUTORIAL_API ARTS_PlayerController : public APlayerController, public IRTS_MarqueeSelection_IF
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float InDeltaTime) override;

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

	void PrimaryAction_Pressed();

	void PrimaryAction_Released();

	// Cast references once for the entire code, reduce system drain.
	void ReferenceCasts();

	void UpdateSelection();

	void GetUnitHUD();

	void UnitMovement();

	void VehicleMovement();

private:
	// How close can the camera get to the root
	float MinZoomLimit = 300.0f;

	// How far can the camera move away from the root
	float MaxZoomLimit = 4000.0f;

	float EdgeScrollSpeedX;

	float EdgeScrollSpeedY;

	float DeltaTime;

public:
	// This is a reference to our camera Pawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	ARTS_CameraPawn* CameraPawnRef;

	ARTS_GameState* GameStateRef;

	ARTS_MarqueeSelection* MarqueeRef;

	AAIController* UnitAI;

	// DisplayUnit HUD Event Dispatcher
	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FDisplayUnitHUD DisplayUnitHUD_Delegate;

	const TArray<TEnumAsByte<EObjectTypeQuery>> SelectableObjectsEnum;

	TArray<ARTS_Cpp_TutorialCharacter*> SelectedUnits;
	TArray<ARTS_Cpp_TutorialCharacter*> SetPassengers;
	TArray<ATP_VehiclePawn*> SelectedGroundVehicles;
	AActor* HitActor;

	ARTS_Cpp_TutorialCharacter* SelectedUnit;
	ATP_VehiclePawn* SelectedGroundVehicle;
	ATP_VehiclePawn* TargetVehicle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
	TSubclassOf<ARTS_Cpp_TutorialCharacter> UnitForDebug;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
	TSubclassOf<ATP_VehiclePawn> VehicleForDebug;

	// How fast the camera moves when zooming
	float ZoomSpeed = 150.0f;

	// How fast the camera can pans
	float PanSpeed = 5.0f;

	// How fast the camera moves
	float DefaultMovementSpeed = 15.0f;

	// Modify default movement speed
	float MovementSpeedModifier = 1.0f;

	float HoldingTime;

	// When mouse is pressed, the camera don't can move
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bDisableCamMovement;

	bool bIsUnitSelected;

	bool bIsBuildingModeActive;

	bool bIsHoldingInput;

	bool bHasPassengers;

	FVector2D MouseLastValidPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Resources")
	int32 WoodResource = 1550;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Resources")
	int32 OreResource = 1200;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Resources")
	int32 MaxResourceLimit;

	int32 PassengerSent;

	FVector TargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
	UMaterial* DecalMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UDecalComponent* PreviousLocationDecal;

	void SetSelectedUnits(TArray<ARTS_Cpp_TutorialCharacter*> InSelectedUnits, TArray<ATP_VehiclePawn*> SelectedVehicles);
	
	TScriptDelegate<FWeakObjectPtr> MovementCompleteDelegate;

	UFUNCTION()
	void AIMoveCompleted();

	void RemoveLocationDecals();

	void GetInVehicle();

	void SpawnUnitDebug();

	void SpawnVehicleDebug();

};