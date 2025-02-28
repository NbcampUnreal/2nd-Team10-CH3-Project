// Fill out your copyright notice in the Description page of Project Settings.
// Copyright Epic Games, Inc. All Rights Reserved.

#include "EnemyAIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BaseEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Use Acceleration when move along path (associated with Animation)
	if (ACharacter* Character1 = Cast<ACharacter>(InPawn))
	{
		if (UCharacterMovementComponent* MovementComp = Character1->GetCharacterMovement())
		{
			MovementComp->bRequestedMoveUseAcceleration=true;
		}
	}

	if (ABaseEnemy* Enemy=Cast<ABaseEnemy>(InPawn))
	{
		if (UBehaviorTree* BT=Enemy->GetBehaviorTree())
		{
			RunBehaviorTree(BT);
			SetAIState(EAIState::Passive);
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

EAIState AEnemyAIController::GetCurrentState() const
{
	return CurrentState; 
}

void AEnemyAIController::SetAIState(EAIState NewState)
{
	CurrentState = NewState;
	if (UBlackboardComponent* BB=GetBlackboardComponent())
	{
		BB->SetValueAsEnum(StateKeyName, static_cast<uint8>(NewState));
	}
}

void AEnemyAIController::SetAttackTarget(AActor* AttackTarget)
{
	if (UBlackboardComponent* BB=GetBlackboardComponent())
	{
		BB->SetValueAsObject(AttackTargetKeyName,AttackTarget);
	}
}

void AEnemyAIController::SetPointOfInterest(const FVector PointOfInterest)
{
	if (UBlackboardComponent* BB=GetBlackboardComponent())
	{
		BB->SetValueAsVector(PointOfInterestKeyName, PointOfInterest);
	}
}

