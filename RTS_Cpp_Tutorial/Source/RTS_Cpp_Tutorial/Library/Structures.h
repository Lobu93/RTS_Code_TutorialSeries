// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Styling/SlateBrush.h"
#include "SlateCore.h"
#include "Structures.generated.h"	

/**
 *  Struct Library
 */

USTRUCT(BlueprintType)
struct FRTS_UnitProfileStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Profile")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Profile")
	int32 Sex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Profile")
	int32 Age;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Profile")
	FSlateBrush UnitImage;
};

USTRUCT(BlueprintType)
struct FGate
{
	GENERATED_BODY()

public:
	FORCEINLINE FGate();
	explicit FORCEINLINE FGate(bool bStartClosed);

	FORCEINLINE void Open() { bGateOpen = true; }

	FORCEINLINE void Close() { bGateOpen = false; }

	FORCEINLINE void Toggle() { bGateOpen = !bGateOpen; }

	FORCEINLINE bool IsOpen() const { return bGateOpen; }

private:
	UPROPERTY(EditAnywhere)
		bool bGateOpen;
};

FORCEINLINE FGate::FGate() : bGateOpen(false) {}

FORCEINLINE FGate::FGate(const bool bStartClosed) : bGateOpen(!bStartClosed) {}