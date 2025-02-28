// Fill out your copyright notice in the Description page of Project Settings.
// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseEnemy.h"
#include "EnemyAIController.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimSequence.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	PatrolPath=nullptr;
	BehaviorTree=nullptr;
	AttackMontage=nullptr;
	Power=0;
	Health=MaxHealth=0.0f;
	Score=0;
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}


float ABaseEnemy::GetHealth() const
{
	return Health;
}

float ABaseEnemy::GetMaxHealth() const
{
	return MaxHealth;
}

void ABaseEnemy::AddHealth(const float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
}

void ABaseEnemy::OnDeath()
{
	// Deliver Score to Game Instance

	Destroy();
}

float ABaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	if (Health <= 0.0f) 
	{
		OnDeath();
	}
	return ActualDamage;
}

APatrolPath* ABaseEnemy::GetPatrolPath() const
{
	return PatrolPath;
}

void ABaseEnemy::SetMovementSpeed(const EMovementSpeed Speed)
{
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		switch (Speed)
		{
			case EMovementSpeed::Idle:
				MovementComp->MaxWalkSpeed = 0.0f;
			case EMovementSpeed::Walking:
				MovementComp->MaxWalkSpeed = 100.0f;
			case EMovementSpeed::Jogging:
				MovementComp->MaxWalkSpeed = 300.0f;
			case EMovementSpeed::Sprinting:
				MovementComp->MaxWalkSpeed = 500.0f;
			default:
				break;
		}
	}
}

UBehaviorTree* ABaseEnemy::GetBehaviorTree() const
{
	return BehaviorTree;
}

void ABaseEnemy::Attack()
{
	//메시 유효 
	if (!GetMesh()) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//둘다 유효
	if (AnimInstance&&AttackMontage)
	{
		//몽타주 실행
		AnimInstance->Montage_Play(AttackMontage);
		//몽타주 끝났을 때 이벤트 바인딩
		AnimInstance->OnMontageEnded.Clear();
		AnimInstance->OnMontageEnded.AddDynamic(this,&ABaseEnemy::OnMontageEnded);
	}
}

void ABaseEnemy::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage==AttackMontage)
	{
		OnAttackEnd.Broadcast();
	}
}

// Called every frame
//void ABaseEnemy::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

// Called to bind functionality to input
//void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

