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