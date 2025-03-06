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
//상태 변경에 따른 이동속도 설정을 위한 함수
void ARangedEnemy::SetMovementSpeed(const EMovementSpeed Speed)
{
	Super::SetMovementSpeed(Speed);
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		switch (Speed)
		{
		case EMovementSpeed::Idle:
			MovementComp->MaxWalkSpeed = 0.0f;
			break;
		case EMovementSpeed::Walking:
			MovementComp->MaxWalkSpeed = 200.0f;
			break;
		case EMovementSpeed::Jogging:
			MovementComp->MaxWalkSpeed = 300.0f;
			break;
		case EMovementSpeed::Sprinting:
			MovementComp->MaxWalkSpeed = 500.0f;
			break;
		default:
			break;
		}
	}
}

void ARangedEnemy::Attack()
{
	//공격 관련 몽타주 실행을 위해 부모 함수 호출
	Super::Attack();
	//격발 관련 함수
	Fire();
}

void ARangedEnemy::Fire()
{
	//지정된 총알이 있는지 검사
	if (!BulletClass) return;
	//메시의 권총 실린더 위치를 총알의 시작위치로 지정
	FVector Start=GetMesh()->GetBoneLocation("pistol_cylinder",EBoneSpaces::WorldSpace);
	//액터 회전 값
	FRotator SpawnRotation=GetActorRotation();
	//플레이어가 유효한지 검사
	if (ACharacter* Player=Cast<ACharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		// 총알 시작위치와 플레이어의 위치를 이용해 방향 벡터 계산 및 정규화
		FVector Direction=(Player->GetActorLocation()-Start).GetSafeNormal();
		//DrawDebugLine(GetWorld(),Start,Start+Direction*5000,FColor::Red,false,1.0f,0,2.0f);
		//촟알 생성
		if (ABaseBullet* Bullet=GetWorld()->SpawnActor<ABaseBullet>(BulletClass,Start,SpawnRotation))
		{
			//총알 데미지 설정
			Bullet->SetBulletDamage(Power);
			//총알의 ProjectileMovement 컴포넌트를 획득
			if (UProjectileMovementComponent* MovementComp=Bullet->GetProjectileComp())
			{
				//Velocity의 방향으로 회전값이 수정되는 옵션 true로 설정 (쉽게 말해 Velocity의 방향으로 발사)
				MovementComp->bRotationFollowsVelocity=true;
				//방향벡터와 원하는 속도를 곱해 Velocity 설정
				MovementComp->Velocity=Direction*MovementComp->GetMaxSpeed();
			}
		}
	}
}

