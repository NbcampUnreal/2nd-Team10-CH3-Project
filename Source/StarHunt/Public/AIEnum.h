// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "AIEnum.generated.h"

UENUM(BlueprintType)
enum class EMovementSpeed : uint8
{
	Idle	UMETA(DisplayName = "Idle"),
	Walking	UMETA(DisplayName = "Walking"),
	Jogging	UMETA(DisplayName = "Jogging"),
	Sprinting	UMETA(DisplayName = "Sprinting")
};

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Passive		UMETA(DisplayName = "Passive"),
	Attacking	UMETA(DisplayName = "Attacking"),
	Frozen		UMETA(DisplayName = "Frozen"),
	Investigating	UMETA(DisplayName = "Investigating"),
	Dead		UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EAISense : uint8
{
	None		UMETA(DisplayName = "None"),
	Sight		UMETA(DisplayName = "Sight"),
	Hearing		UMETA(DisplayName = "Hearing"),
	Damage		UMETA(DisplayName = "Damage")
};