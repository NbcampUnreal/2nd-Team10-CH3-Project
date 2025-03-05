// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HealingItemActivate.h"
#include "ActivateItem.generated.h"

/**
 * 
 */
UCLASS()
class STARHUNT_API UActivateItem : public UObject
{
	GENERATED_BODY()
	
public:
	UActivateItem();
	IActivateItemInterface* GetActivateActionClass();
};
