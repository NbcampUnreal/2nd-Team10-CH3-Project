// Fill out your copyright notice in the Description page of Project Settings.
// Copyright Epic Games, Inc. All Rights Reserved.

#include "EnemyAIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/NavMovementComponent.h"

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Use Acceleration when move along path (associated with Animation)
	ACharacter* Character1 = Cast<ACharacter>(InPawn);
	if (Character1)
	{
		UCharacterMovementComponent* MovementComp = Character1->GetCharacterMovement();
		if (MovementComp)
		{
			MovementComp->bRequestedMoveUseAcceleration=true;
		}
	}
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
}
