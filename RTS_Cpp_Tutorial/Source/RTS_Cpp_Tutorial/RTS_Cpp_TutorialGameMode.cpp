// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTS_Cpp_TutorialGameMode.h"
#include "RTS_Cpp_TutorialPlayerController.h"
#include "RTS_Cpp_TutorialCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARTS_Cpp_TutorialGameMode::ARTS_Cpp_TutorialGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ARTS_Cpp_TutorialPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}