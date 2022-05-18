// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS_FuncLib.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"

float URTS_FuncLib::SetGameTime(float DeltaTime, float GameSpeed, float InitialGameTime, int32& DayCount)
{
	float DividendLocal;
	float DivisorLocal = 24.0f;
	float RemainderLocal;

	if (GameSpeed > 0.0f)
	{
		DividendLocal = (DeltaTime / GameSpeed) + InitialGameTime;

		// Division (Whole and Reminder)
		DayCount = UKismetMathLibrary::FMod(DividendLocal, DivisorLocal, RemainderLocal);

		return RemainderLocal;
	}
	else
	{
		return InitialGameTime;
	}
}

FVector URTS_FuncLib::SetCursorWorldPosition(APlayerController* Controller, float SightDistance)
{
	FVector ReturnLocal = FVector(0.0f, 0.0f, 0.0f);

	if (UWorld* World = Controller->GetWorld())
	{
		FHitResult HitResult;
		FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
		FVector StartLocation;
		FVector EndLocation;

		Controller->DeprojectMousePositionToWorld(StartLocation, EndLocation);
		EndLocation = StartLocation + (EndLocation * SightDistance);

		World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);

		if (HitResult.Actor != nullptr)
		{
			return HitResult.Location;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("URTS_FuncLib::SetCursorWorldPosition() | Get World object fail..."));

			return ReturnLocal;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("URTS_FuncLib::SetCursorWorldPosition() | Get World object fail..."));

		return ReturnLocal;
	}
}
