// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	FString ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture* InventoryIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StockMax;
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
	TSet<FString> FixtureTypes;

};

USTRUCT(BlueprintType)
struct FGunFixtureItemStateRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FixtureType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageAmount;

};
