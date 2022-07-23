// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "../Library/EnumList.h"
#include "TP_VehiclePawn.generated.h"

class UWheeledVehicleMovementComponent4W;
class ARTS_PlayerController;
class ARTS_Cpp_TutorialCharacter;

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
	bool bHasPassengers;
	bool bIsMoving;
	bool bIsHidden;
	FVector Target;
	float InitialDistance;
	float StartingTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gambiarra")
	bool bCanDisplayVehicleHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle")
	int32 MaxPassengers = 4;
	
	TArray<ARTS_Cpp_TutorialCharacter*> PassengersTemp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle")
	TArray<ARTS_Cpp_TutorialCharacter*> CurrentPassengers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle")
	EVehicleTypes VehicleTypeEnum = EVehicleTypes::Transport;

	void SetSelectedDecal();

	void SetDeselectedDecal();

	void MoveToLocation(FVector TargetLocation);

	UFUNCTION()
	void UpdateMovement();

	void ReceiveMoveCommand(FVector TargetLocation);

	void GetPassengers(TArray<ARTS_Cpp_TutorialCharacter*> Passengers);

	UFUNCTION(BlueprintCallable, Category = "Passengers")
	void RemovePassengers();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Vehicle")
	void DisplayVehicleHUD(AActor* Actor, bool bBypass);
	virtual void DisplayVehicleHUD_Implementation(AActor* Actor, bool bBypass);

	// For debug purpose only
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	AActor* ActorToMove;
};

PRAGMA_ENABLE_DEPRECATION_WARNINGS

