// Fill out your copyright notice in the Description page of Project Settings.
// Copyright Epic Games, Inc. All Rights Reserved.

#include "MeleeEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIEnum.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

AMeleeEnemy::AMeleeEnemy()
{
	Score = 100;
	Health = 500;
	MaxHealth = Health;
	Power=20;
	AttackRadius=150.0f;
	DefendRadius=350.0f;
	AttackRange=200.0f;
}

void AMeleeEnemy::BeginPlay()
{
	Super::BeginPlay();

	//애니메이션 Notify 이벤트 연결
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this,&AMeleeEnemy::HandleAttackMontageNotify);
	}
}

void AMeleeEnemy::HandleAttackMontageNotify(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	//Notify Name 확인
	if (NotifyName=="Slash")
	{
		//범위 설정
		FVector Start=GetActorLocation();
		FVector End=Start+GetActorForwardVector()*AttackRange;
		float Radius =50.0f;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		TArray<FHitResult> OutHits;
		//충돌 여부 검사
		bool bHit=GetWorld()->SweepMultiByObjectType
		(
			OutHits,
			Start,
			End,
			FQuat::Identity,
			FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn),
			FCollisionShape::MakeSphere(Radius),
			Params
		);
		// 공격 범위에 걸린 액터들 중에 Player의 클래스에 해당하는게 있다면 데미지 적용
		if (bHit)
		{
			for (auto& Hit:OutHits)
			{
				if (AActor* HitActor=Hit.GetActor())
				{
					if (ACharacter* HitCharacter=Cast<ACharacter>(HitActor))
					{
						if (APlayerCharacter* Player=Cast<APlayerCharacter>(HitCharacter))
						{
							UGameplayStatics::ApplyDamage
							(
								Player,
								Power,
								GetController(),
								this,
								UDamageType::StaticClass()
							);
						}
					}
				}
			}
		}
	}
}

void AMeleeEnemy::SetMovementSpeed(const EMovementSpeed Speed)
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

