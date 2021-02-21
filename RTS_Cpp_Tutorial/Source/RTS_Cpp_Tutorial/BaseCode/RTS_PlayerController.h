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

public:
	// This is a reference to our camera Pawn
	ARTS_CameraPawn* CameraPawnRef;

};
