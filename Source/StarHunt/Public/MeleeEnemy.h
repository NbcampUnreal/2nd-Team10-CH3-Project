// Fill out your copyright notice in the Description page of Project Settings.
// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "MeleeEnemy.generated.h"

/**
 * 
 */
UCLASS()
class STARHUNT_API AMeleeEnemy : public ABaseEnemy
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Attack")
	float AttackRange;
	
public:
	AMeleeEnemy();
	void BeginPlay() override;
	virtual void SetMovementSpeed(EMovementSpeed Speed) override;

	UFUNCTION()
	void HandleAttackMontageNotify(FName NotifyName, const FBranchingPointNotifyPayload& Payload);
};
