// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "../Library/RTS_GameTime_IF.h"
#include "RTS_GameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameSpeedControl, float, InGameTime);

class ARTS_PlayerController;

/**
 * 
 */
UCLASS()
class RTS_CPP_TUTORIAL_API ARTS_GameState : public AGameStateBase, public IRTS_GameTime_IF
{
	GENERATED_BODY()

	ARTS_GameState();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ReferenceCasts();

	void SetClock();

	void SetCalendar();

private:
	float GameSpeed = 150.0f;
	float GameTime;
	float DefaultGameSpeed = 0.025f; // 150.0f;
	float SpeedMultiplier = 1.0f;
	int32 DayCounter;
	TArray<int32> GameDate;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ARTS_PlayerController* ControllerRef;

	// bool bIsImplemented;

	int32 Day = 28;
	UPROPERTY(EditAnywhere, Category = "Calendar", meta = (ClampMin = "1.0", ClampMax = "12.0", UIMin = "1.0", UIMax = "12.0"))
	int32 Month = 3;
	int32 Year = 2021;
	int32 Hours = 11; // int variable that holds hours game time
	int32 Minutes; // int variable that holds minutes game time
	int32 Seconds; // int variable that holds seconds game time

	//Event Dispatcher
	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FGameSpeedControl GameSpeedControl_Delegate;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Game Speed Settings")
		bool FunctionUpdateGameSpeed(float InSpeedMultiplier);
	virtual bool FunctionUpdateGameSpeed_Implementation(float InSpeedMultiplier) override;

	float GetGameSpeed();
};
