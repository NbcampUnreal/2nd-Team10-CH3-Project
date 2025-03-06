// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AimingSystem.generated.h"

/**
 *
 */
UCLASS()
class STARHUNT_API UAimingSystem : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static FVector GetTargetLocation(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
	static FRotator GetTargetRotation(APlayerController* PlayerController, const FVector& SpawnLocation);

	UFUNCTION(BlueprintCallable)
	static void SetDebugMode(bool value);
private:
	static bool bDebugMode;
};