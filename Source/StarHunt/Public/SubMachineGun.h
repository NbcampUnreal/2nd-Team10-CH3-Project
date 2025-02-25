// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGun.h"
#include "SubMachineGun.generated.h"

/**
 * 
 */
UCLASS()
class STARHUNT_API ASubMachineGun : public ABaseGun
{
	GENERATED_BODY()
	
public:
	ASubMachineGun();

	virtual void Fire() override;

	virtual void StopFire() override;
};
