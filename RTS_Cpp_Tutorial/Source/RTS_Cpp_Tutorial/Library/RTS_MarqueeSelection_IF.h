// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RTS_MarqueeSelection_IF.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URTS_MarqueeSelection_IF : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RTS_CPP_TUTORIAL_API IRTS_MarqueeSelection_IF
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	void OnInputStart();

	void OnInputHold();

	void OnInputRelease(float HoldTime);
};
