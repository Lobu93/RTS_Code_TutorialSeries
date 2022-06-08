// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "../Library/RTS_MarqueeSelection_IF.h"
#include "RTS_MarqueeSelection.generated.h"

class ARTS_Cpp_TutorialCharacter;
class ARTS_PlayerController;

/**
 * 
 */
UCLASS()
class RTS_CPP_TUTORIAL_API ARTS_MarqueeSelection : public AHUD, public IRTS_MarqueeSelection_IF
{
	GENERATED_BODY()
	
protected:
	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	ARTS_PlayerController* ControllerRef;

	UPROPERTY()
	TArray<ARTS_Cpp_TutorialCharacter*> SelectedUnits;

	FVector2D ClickedLocation;
	FVector2D HoldingLocation;
	bool bIsDrawing;

	void OnInputStart();

	void OnInputRelease(float HoldTime);

	void OnInputHold();

	void DrawMarquee();
};
