// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RarityInterface.generated.h"

UENUM(BlueprintType)
enum class ERarity : uint8
{
    Common     UMETA(DisplayName = "Common"),
    Rare       UMETA(DisplayName = "Rare"),
    Epic       UMETA(DisplayName = "Epic"),
    Legendary  UMETA(DisplayName = "Legendary")
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URarityInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STARHUNT_API IRarityInterface
{
	GENERATED_BODY()

public:

};
