// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

UENUM(BlueprintType)
enum class EMonthNames : uint8
{
	_blank,
	January,
	February,
	March,
	April,
	May,
	June,
	July,
	August,
	September,
	October,
	November,
	December
};

UENUM(BlueprintType)
enum class EGameSpeedSettings : uint8
{
	Speed_0,
	Speed_1,
	Speed_2,
	Speed_3,
	Speed_4,
	Speed_5
};

UENUM(BlueprintType)
enum class EResourceLevel : uint8
{
	Resource_0,
	Resource_1,
	Resource_2,
	Resource_3,
	Resource_4,
	Resource_5,
	Resource_6
};