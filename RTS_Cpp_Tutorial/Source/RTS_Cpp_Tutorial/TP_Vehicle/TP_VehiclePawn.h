// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "TP_VehiclePawn.generated.h"

class UWheeledVehicleMovementComponent4W;
class ARTS_PlayerController;

PRAGMA_DISABLE_DEPRECATION_WARNINGS

UCLASS(config=Game)
class ATP_VehiclePawn : public AWheeledVehicle
{
	GENERATED_BODY()
	
public:
	ATP_VehiclePawn();

	/** Are we in reverse gear */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly)
	bool bInReverseGear;

	// Begin Actor interface
	virtual void Tick(float Delta) override;

protected:
	virtual void BeginPlay() override;

	UWheeledVehicleMovementComponent4W* Vehicle4W;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* SelectedDecal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* DeselectedDecal;

private:

	/* Are we on a 'slippery' surface */
	bool bIsLowFriction;

public:
	ARTS_PlayerController* ControllerRef;
	FTimerHandle TimerHandleGlobal;
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

	// For debug purpose only
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	AActor* ActorToMove;
};

PRAGMA_ENABLE_DEPRECATION_WARNINGS

