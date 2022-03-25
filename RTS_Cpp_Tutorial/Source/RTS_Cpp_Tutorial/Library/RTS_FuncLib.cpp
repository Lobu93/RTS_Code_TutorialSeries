// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS_FuncLib.h"
#include "Kismet/KismetMathLibrary.h"

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