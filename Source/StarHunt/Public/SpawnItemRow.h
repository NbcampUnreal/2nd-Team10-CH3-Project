// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemStateRow.h"
#include "SpawnItemRow.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FSpawnItemRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnRate;
};
