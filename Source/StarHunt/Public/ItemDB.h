// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemStateRow.h"
#include "ItemDB.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,  ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class STARHUNT_API UItemDB : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSystem|ItemData")
	UDataTable* BaseItemDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSystem|ItemData")
	UDataTable* HealingItemDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSystem|ItemData")
	UDataTable* GunItemDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSystem|ItemData")
	UDataTable* GunFixtureItemDataTable;

	FBaseItemStateRow* GetBaseItemStateRow(const FString& ItemId);
	FHealingItemStateRow* GetHealingItemStateRow(const FString& ItemId);
	FGunItemStateRow* GetGunItemStateRow(const FString& ItemId);
	FGunFixtureItemStateRow* GetGunFixtureItemStateRow(const FString& ItemId);
};
