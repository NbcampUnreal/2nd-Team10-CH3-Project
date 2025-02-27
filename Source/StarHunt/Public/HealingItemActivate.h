// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ActivateItemInterface.h"
#include "ItemStateRow.h"
#include "HealingItemActivate.generated.h"

/**
 * 
 */
UCLASS()
class STARHUNT_API UHealingItemActivate : public UObject, public IActivateItemInterface
{
	GENERATED_BODY()
public:
	UHealingItemActivate();
	UHealingItemActivate(FHealingItemStateRow& HealingItemStateRow);
	// IActivateItemInterface을(를) 통해 상속됨
	void ActivateItem(AActor* Activator) override;

private:
	float HealingAmount;
};
