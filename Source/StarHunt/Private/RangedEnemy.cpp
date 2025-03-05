// Fill out your copyright notice in the Description page of Project Settings.
// Copyright Epic Games, Inc. All Rights Reserved.

#include "RangedEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseBullet.h"
#include "Kismet/GameplayStatics.h"
#include "AIEnum.h"
#include "GameFramework/ProjectileMovementComponent.h"

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
	Fire();
}

void ARangedEnemy::Fire()
{
	if (!BulletClass) return;
	FVector Start=GetMesh()->GetBoneLocation("pistol_cylinder",EBoneSpaces::WorldSpace);
	FRotator SpawnRotation=GetActorRotation();

	if (ACharacter* Player=Cast<ACharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		FVector Direction=(Player->GetActorLocation()-Start).GetSafeNormal();
		DrawDebugLine(GetWorld(),Start,Start+Direction*5000,FColor::Red,false,1.0f,0,2.0f);
		if (ABaseBullet* Bullet=GetWorld()->SpawnActor<ABaseBullet>(BulletClass,Start,SpawnRotation))
		{
			Bullet->SetBulletDamage(Power);
			if (UProjectileMovementComponent* MovementComp=Bullet->GetProjectileComp())
			{
				MovementComp->bRotationFollowsVelocity=true;
				MovementComp->Velocity=Direction*MovementComp->GetMaxSpeed();
			}
		}
	}
}

