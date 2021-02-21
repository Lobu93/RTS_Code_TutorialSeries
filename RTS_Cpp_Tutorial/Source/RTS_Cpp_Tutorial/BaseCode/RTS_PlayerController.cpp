// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS_PlayerController.h"
#include "RTS_CameraPawn.h"
#include "Kismet/GameplayStatics.h"
// #include "Engine/World.h"

// Called when the game starts or when spawned
void ARTS_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Cast references once for the entire code, reduce system drain.
	CameraPawnRef = (ARTS_CameraPawn*)UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!CameraPawnRef)
	{
		UE_LOG(LogTemp, Error, TEXT("ARTS_PlayerController::BeginPlay() Bad CameraPawn Class"));
	}
}
