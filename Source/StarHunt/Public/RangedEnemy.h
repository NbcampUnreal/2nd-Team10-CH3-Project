// Fill out your copyright notice in the Description page of Project Settings.
// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "RangedEnemy.generated.h"

/**
 * 
 */
UCLASS()
class STARHUNT_API ARangedEnemy : public ABaseEnemy
{
	GENERATED_BODY()
	
public:
	ARangedEnemy();

	virtual void SetMovementSpeed(const EMovementSpeed Speed) override;

	virtual void Attack() override;
};
