// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "RTS_GroundVehicleMaster.generated.h"

class UChaosWheeledVehicleMovementComponent;
class ARTS_PlayerController;

/**
 * 
 */
UCLASS()
class RTS_CPP_TUTORIAL_API ARTS_GroundVehicleMaster : public AWheeledVehiclePawn
{
	GENERATED_BODY()
	
public:
	ARTS_GroundVehicleMaster();

protected:
	virtual void BeginPlay() override;

	UChaosWheeledVehicleMovementComponent* VehicleMovComp;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* SelectedDecal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* DeselectedDecal;

public:
	ARTS_PlayerController* ControllerRef;
	bool bIsSelected;
	FVector Target;
	float InitialDistance;
	float StartingTime;

	void SetSelectedDecal();

	void SetDeselectedDecal();

	void MoveToLocation(FVector TargetLocation);

	UFUNCTION()
	void UpdateMovement();

	void ReceiveMoveCommand(FVector TargetLocation);
};
