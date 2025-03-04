// Fill out your copyright notice in the Description page of Project Settings.
// Copyright Epic Games, Inc. All Rights Reserved.

#include "RangedEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIEnum.h"

ARangedEnemy::ARangedEnemy()
{
	Score = 150;
	Health = 300;
	MaxHealth = Health;
	Power=10;
	AttackRadius=600.0f;
	DefendRadius=600.0f;
}

void ARangedEnemy::SetMovementSpeed(const EMovementSpeed Speed)
{
	Super::SetMovementSpeed(Speed);
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		switch (Speed)
		{
		case EMovementSpeed::Idle:
			MovementComp->MaxWalkSpeed = 0.0f;
		case EMovementSpeed::Walking:
			MovementComp->MaxWalkSpeed = 200.0f;
		case EMovementSpeed::Jogging:
			MovementComp->MaxWalkSpeed = 300.0f;
		case EMovementSpeed::Sprinting:
			MovementComp->MaxWalkSpeed = 500.0f;
		default:
			break;
		}
	}
}

void ARangedEnemy::Attack()
{
	Super::Attack();
}

