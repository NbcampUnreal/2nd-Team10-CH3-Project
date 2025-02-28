// Fill out your copyright notice in the Description page of Project Settings.
// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIEnum.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */

UCLASS()
class STARHUNT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()


protected:
	const FName StateKeyName="State";
	const FName AttackTargetKeyName="AttackTarget";
	const FName PointOfInterestKeyName="PointOfInterest";

	UPROPERTY(VisibleAnywhere,Category="AI")
	EAIState CurrentState;
	
public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	UFUNCTION(BlueprintPure,Category="AI")
	EAIState GetCurrentState() const;
	
	UFUNCTION(BlueprintCallable,Category="AI")
	void SetAIState(EAIState NewState);
	
	UFUNCTION(BlueprintCallable,Category="AI")
	void SetAttackTarget(AActor* AttackTarget);

	UFUNCTION(BlueprintCallable,Category="AI")
	void SetPointOfInterest(const FVector PointOfInterest);
};
