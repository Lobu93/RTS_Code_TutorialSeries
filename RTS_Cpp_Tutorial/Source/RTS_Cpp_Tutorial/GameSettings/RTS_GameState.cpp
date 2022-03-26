// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS_GameState.h"
#include "../Player/RTS_PlayerController.h"
#include "../Library/RTS_FuncLib.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Library/RTS_GameTime_IF.h"


ARTS_GameState::ARTS_GameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARTS_GameState::BeginPlay()
{
	Super::BeginPlay();

	ReferenceCasts();

	GameTime = (float)Hours + ((float)Minutes / 60.0f) + ((float)Seconds / 3600.0f);

	GameDate.Add(Day);
	GameDate.Add(Month);
	GameDate.Add(Year);

	GameHour.Add(0);
	GameHour.Add(0);
	GameHour.Add(Hours);

	GameSpeed = DefaultGameSpeed;

}

// Called every frame
void ARTS_GameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GameTime = URTS_FuncLib::SetGameTime(DeltaTime, GameSpeed, GameTime, DayCounter);

	SetClock();

	SetCalendar();
}

bool ARTS_GameState::FunctionUpdateGameSpeed_Implementation(float InSpeedMultiplier)
{
	SpeedMultiplier = InSpeedMultiplier;

	GameSpeed = SpeedMultiplier * DefaultGameSpeed;

	if (GameSpeedControl_Delegate.IsBound())
	{
		GameSpeedControl_Delegate.Broadcast(GameSpeed);
	}

	// UE_LOG(LogTemp, Warning, TEXT("ARTS_GameState::FunctionUpdateGameSpeed_Implementation() | SpeedMultiplier: %f GameSpeed: %f"), SpeedMultiplier, GameSpeed);

	return true;
}

void ARTS_GameState::ReferenceCasts()
{
	// Cast references once for the entire code, reduce system drain.
	ControllerRef = (ARTS_PlayerController*)GetWorld()->GetFirstPlayerController();
	if (!ControllerRef)
	{
		UE_LOG(LogTemp, Error, TEXT("ARTS_GameState::ReferenceCasts() Bad CameraPawn Class"));
	}
}

void ARTS_GameState::SetClock_Implementation()
{
	float DividendLocal;
	float DivisorSecsAndMinsLocal = 60.0f;
	float DivisorHoursLocal = 24.0f;
	float RemainderLocal;
	int32 ReturnValueLocal;

	DividendLocal = GameTime * 3600.0f;

	// Division (Whole and Remainder). Remainder = Seconds
	ReturnValueLocal = UKismetMathLibrary::FMod(DividendLocal, DivisorSecsAndMinsLocal, RemainderLocal);

	Seconds = UKismetMathLibrary::FFloor(RemainderLocal);

	DividendLocal = (float)ReturnValueLocal;

	// Division (Whole and Remainder). Remainder = Minutes
	ReturnValueLocal = UKismetMathLibrary::FMod(DividendLocal, DivisorSecsAndMinsLocal, RemainderLocal);

	Minutes = UKismetMathLibrary::FFloor(RemainderLocal);

	DividendLocal = (float)ReturnValueLocal;

	// Division (Whole and Remainder). Remainder = Hours
	ReturnValueLocal = UKismetMathLibrary::FMod(DividendLocal, DivisorHoursLocal, RemainderLocal);

	Hours = UKismetMathLibrary::FFloor(RemainderLocal);

	//// Make GameDate array Seconds slot
	//GameDate[0] = Seconds;

	//// Make GameDate array Minutes slot
	//GameDate[1] = Minutes;

	//// Make GameDate array Hours slot
	//GameDate[2] = Hours;

	// Make GameDate array Seconds slot
	GameHour[0] = Seconds;

	// Make GameDate array Minutes slot
	GameHour[1] = Minutes;

	// Make GameDate array Hours slot
	GameHour[2] = Hours;

	// Debug session
	//FString MessageString; // = FString("Move Right. Because of Mouse X value is ");

	//MessageString.AppendInt(GameHour[2]);
	//MessageString.Append(TEXT(":"));
	//MessageString.AppendInt(GameHour[1]);
	//MessageString.Append(TEXT(":"));
	//MessageString.AppendInt(GameHour[0]);
	//GEngine->AddOnScreenDebugMessage(-1, 30, FColor::White, MessageString);

	/*MessageString.AppendInt(GameDate[2]);
	MessageString.Append(TEXT(":"));
	MessageString.AppendInt(GameDate[1]);
	MessageString.Append(TEXT(":"));
	MessageString.AppendInt(GameDate[0]);
	GEngine->AddOnScreenDebugMessage(-1, 30, FColor::White, MessageString);*/
}

void ARTS_GameState::SetCalendar_Implementation()
{
	int32 DaysInMonthLocal;

	Day = Day + DayCounter;

	DaysInMonthLocal = UKismetMathLibrary::DaysInMonth(Year, Month);

	if (Day > DaysInMonthLocal)
	{
		Day = 1;

		Month++;
	}
	
	if (Month > 12)
	{
		Month = 1;

		Year++;
	}

	// Make GameDate array Seconds slot
	GameDate[0] = Day;

	// Make GameDate array Minutes slot
	GameDate[1] = Month;

	// Make GameDate array Hours slot
	GameDate[2] = Year;

	// Debug session
	/*FString MessageString;

	MessageString.AppendInt(GameDate[0]);
	MessageString.Append(TEXT("/"));
	MessageString.AppendInt(GameDate[1]);
	MessageString.Append(TEXT("/"));
	MessageString.AppendInt(GameDate[2]);
	GEngine->AddOnScreenDebugMessage(-1, 30, FColor::Red, MessageString);*/
}

float ARTS_GameState::GetGameSpeed()
{
	return GameSpeed;
}
