// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AimingSystem.generated.h"

/**
 * 
 */
UCLASS()
class STARHUNT_API UAimingSystem : public UObject
{
	GENERATED_BODY()
	
public:
	UAimingSystem();

	UFUNCTION(BlueprintCallable)
	FVector GetTargetLocation(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
	FRotator GetTargetRotation(APlayerController* PlayerController, const FVector& SpawnLocation);

	void SetDebugMode(bool bIsDebugmode);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
	bool bDebug;
};
