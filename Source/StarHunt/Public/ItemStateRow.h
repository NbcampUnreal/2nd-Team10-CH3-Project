// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemEnumClass.h"
#include "ItemStateRow.generated.h"
/**
 * 
 */



USTRUCT(BlueprintType)
struct FBaseItemStateRow : public FTableRowBase
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<AActor> SpawnActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInventoryType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* InventoryIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StockMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERarity Rarity;
};

USTRUCT(BlueprintType)
struct FHealingItemStateRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealingAmount;
};

USTRUCT(BlueprintType)
struct FGunItemStateRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageAmount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGunType GunType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<EGunFixtureType> FixtureTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<AActor> GunSoftClass;
	

};

USTRUCT(BlueprintType)
struct FGunFixtureItemStateRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGunFixtureType FixtureType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAmmoAmount;

	FGunFixtureItemStateRow operator+=(const FGunFixtureItemStateRow& Other)
	{
		DamageAmount += Other.DamageAmount;
		MaxAmmoAmount += Other.MaxAmmoAmount;
		return *this;
	}
};