// Fill out your copyright notice in the Description page of Project Settings.


#include "SunRotation.h"
#include "Engine/World.h"
#include "../GameSettings/RTS_GameState.h"
#include "../Library/RTS_FuncLib.h"

// Sets default values
ASunRotation::ASunRotation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
}

// Called when the game starts or when spawned
void ASunRotation::BeginPlay()
{
	Super::BeginPlay();

	GameStateRef = (ARTS_GameState*)GetWorld()->GetGameState();

	if (GameStateRef)
	{
		GameStateRef->GameSpeedControl_Delegate.AddDynamic(this, &ASunRotation::GetGameSpeed);

		GameSpeed = GameStateRef->GetGameSpeed();
	}
}

void ASunRotation::GetGameSpeed(float InGameSpeed)
{
	GameSpeed = InGameSpeed;
}

// Called every frame
void ASunRotation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float Y_PitchLocal;
	float GameTimeLocal;
	int32 DayCounterLocal;

	GameTimeLocal = URTS_FuncLib::SetGameTime(DeltaTime, GameSpeed, GameTime, DayCounterLocal);

	Y_PitchLocal = GameTimeLocal / 1.5f;

	Y_PitchLocal /= 6.0f;

	// UE_LOG(LogTemp, Warning, TEXT("void ASunRotation::Tick() | Y_PitchLocal pre *= 90: %f"), Y_PitchLocal);

	Y_PitchLocal *= 90.0f;

	SunRef->AddActorLocalRotation(FRotator(Y_PitchLocal, 0.0f, 0.0f));
}

