// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RTS_FuncLib.generated.h"

/**
 * 
 */
UCLASS()
class RTS_CPP_TUTORIAL_API URTS_FuncLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// This is a function to help run and pass game time using hours
	UFUNCTION(BlueprintCallable, Category = "Library")
		static float SetGameTime(float DeltaTime, float GameSpeed, float InitialGameTime, int32& DayCount);
};
