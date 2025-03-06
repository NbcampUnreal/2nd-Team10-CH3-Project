// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGun.h"
#include "Rifle.generated.h"

/**
 * 
 */
UCLASS()
class STARHUNT_API ARifle : public ABaseGun
{
	GENERATED_BODY()

public:
	ARifle();

	// virtual void Fire() override;

	virtual void StopFire() override;
};
