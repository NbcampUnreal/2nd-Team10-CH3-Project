// Fill out your copyright notice in the Description page of Project Settings.
// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseEnemy.h"
#include "EnemyAIController.h"
#include "ItemSpawnComponent.h"
#include "AIEnum.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimSequence.h"

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
	AttackRadius=0.0f;
	DefendRadius=0.0f;
	bIsDead=false;
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

	if (USkeletalMeshComponent* SMesh=GetMesh())
	{
		//래그돌 효과
		SMesh->SetSimulatePhysics(true);
		SMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	
	if (AEnemyAIController* EnemyController=Cast<AEnemyAIController>(GetController()))
	{
		//상태 Dead로 변경
		EnemyController->SetAIState(EAIState::Dead);
		bIsDead=true;
		if (UBrainComponent* Brain=EnemyController->GetBrainComponent())
		{
			//BehaviorTree 동작 중단
			Brain->StopLogic("Dead");
		}
	}

	//Destroy() 3초 뒤 호출
	FTimerHandle DestroyTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		DestroyTimerHandle,
		this,
		&ABaseEnemy::DelayedDestroy,
		3.0f,
		false
	);
}

void ABaseEnemy::DelayedDestroy()
{
	Destroy();
}

float ABaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (AEnemyAIController* AIController=Cast<AEnemyAIController>(GetController()))
	{
		AIController->SetAIState(EAIState::Frozen);
		AIController->SetAttackTarget(DamageCauser);
	}
	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	if (Health <= 0.0f) 
	{
		OnDeath();
	}
	return ActualDamage;
}

void ABaseEnemy::SetMovementSpeed(const EMovementSpeed Speed)
{
	
}

APatrolPath* ABaseEnemy::GetPatrolPath() const
{
	return PatrolPath;
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

float ABaseEnemy::GetAttackRadius() const
{
	return AttackRadius;
}

float ABaseEnemy::GetDefendRadius() const
{
	return DefendRadius;
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

