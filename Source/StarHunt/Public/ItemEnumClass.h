// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemEnumClass.generated.h"
/**
 * 
 */


UENUM(BlueprintType)
enum class EInventoryType : uint8
{
	Gun UMETA(DisplayName = "Gun"),
	GunFixture UMETA(DisplayName = "GunFixture"),
	Consumable UMETA(DisplayName = "Consumable")
};

UENUM(BlueprintType)
enum class EGunFixtureType : uint8
{
	Muffle UMETA(DisplayName = "Muffle"),
	Magazine UMETA(DisplayName = "Magazine")
};

UENUM(BlueprintType)
enum class ERarity : uint8
{
	Common     UMETA(DisplayName = "Common"),
	Rare       UMETA(DisplayName = "Rare"),
	Epic       UMETA(DisplayName = "Epic"),
	Legendary  UMETA(DisplayName = "Legendary")
};

UENUM(BlueprintType)
enum class EGunType : uint8
{
	Pistol     UMETA(DisplayName = "Pistol"),
	Rifle       UMETA(DisplayName = "Rifle"),
	ShotGun       UMETA(DisplayName = "ShotGun")
};