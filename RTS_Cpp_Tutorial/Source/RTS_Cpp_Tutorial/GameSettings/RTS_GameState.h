// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "../Library/RTS_GameTime_IF.h"
#include "RTS_GameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameSpeedControl, float, InGameTime);

class ARTS_PlayerController;
class ARTS_Cpp_TutorialCharacter;

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

private:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	//float GameSpeed = 0.25f; // For debug change: 150.0f;
	float GameTime;
	float DefaultGameSpeed = 150.0f; // For debug change: 150.0f;
	float SpeedMultiplier = 1.0f;
	int32 DayCounter;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	ARTS_PlayerController* ControllerRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSpeedSettings")
	float GameSpeed = 150.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	TArray<int32> GameDate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	TArray<int32> GameHour;

	int32 Day = 17;
	UPROPERTY(EditAnywhere, Category = "Calendar", meta = (ClampMin = "1.0", ClampMax = "12.0", UIMin = "1.0", UIMax = "12.0"))
	int32 Month = 3;
	int32 Year = 2021;
	int32 Hours = 1; // int variable that holds hours game time
	int32 Minutes; // int variable that holds minutes game time
	int32 Seconds; // int variable that holds seconds game time

	TArray<ARTS_Cpp_TutorialCharacter*> ListOfAllUnits;

	//Event Dispatcher
	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FGameSpeedControl GameSpeedControl_Delegate;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Game Speed Settings")
	bool FunctionUpdateGameSpeed(float InSpeedMultiplier);
	virtual bool FunctionUpdateGameSpeed_Implementation(float InSpeedMultiplier) override;


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Calendar")
	void SetClock();
	virtual void SetClock_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Calendar")
	void SetCalendar();
	virtual void SetCalendar_Implementation();

	UFUNCTION(BlueprintCallable)
	float GetGameSpeed();

	TArray<ARTS_Cpp_TutorialCharacter*> GetAllUnits();
};
