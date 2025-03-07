// Fill out your copyright notice in the Description page of Project Settings.
// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseEnemy.h"
#include "EnemyAIController.h"
#include "ItemSpawnComponent.h"
#include "ShooterGameInstance.h"
#include "AIEnum.h"
#include "BossEnemy.h"
#include "PlayerCharacter.h"
#include "ShooterGameStateBase.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimSequence.h"
#include "Perception/AISense_Damage.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	PatrolPath=nullptr;
	BehaviorTree=nullptr;
	AttackMontage=nullptr;
	HitMontage=nullptr;
	Power=0;
	Health=MaxHealth=0.0f;
	Score=0;
	AttackRadius=0.0f;
	DefendRadius=0.0f;
	bIsDead=false;
	

	// //HP Bar 설정
	// HPBar=CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar"));
	// HPBar->SetupAttachment(RootComponent);
	// HPBar->SetWidgetSpace(EWidgetSpace::World);

	// Item
	ItemSpawnComponent = CreateDefaultSubobject<UItemSpawnComponent>(TEXT("ItemSpawnComponemt"));
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// UpdateHPBar();
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
	// UpdateHPBar();
}

void ABaseEnemy::OnDeath()
{
	// Deliver Score to Game Instance
	if (UWorld* World=GetWorld())
	{
		ItemSpawnComponent->SpawnedItem();
		if (UShooterGameInstance* GameInstance=Cast<UShooterGameInstance>(World->GetGameInstance()))
		{
			GameInstance->AddToScore(Score);
		}
	}
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
	if (ABossEnemy* Boss=Cast<ABossEnemy>(GetController()->GetPawn()))
	{
		UE_LOG(LogTemp, Display, TEXT("Boss Detected"));
		if (AShooterGameStateBase* GameState=Cast<AShooterGameStateBase>(GetWorld()->GetGameState()))
		{
			GameState->OnGameClear();
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

// void ABaseEnemy::UpdateHPBar() const
// {
// 	if (!HPBar) return;
//
// 	if (UUserWidget* WidgetInstance=HPBar->GetUserWidgetObject())
// 	{
// 		if (UProgressBar* HP=Cast<UProgressBar>(WidgetInstance->GetWidgetFromName(TEXT("HPBar"))))
// 		{
// 			const float Percent= MaxHealth>0.0f?Health/MaxHealth:0.0f;
// 			HP->SetPercent(Percent);
// 		}
// 	}
// 	
// }
//
// void ABaseEnemy::Tick(float DeltaSeconds)
// {
// 	Super::Tick(DeltaSeconds);
//
// 	if (!HPBar&&!GetWorld()) return;
// 	
// 	APlayerCharacter* Player=Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
// 	if (!Player) return;
// 	//플레이어 위치
// 	FVector PlayerLocation=Player->GetActorLocation();
// 	//몬스터의 위치
// 	FVector EnemyLocation=GetActorLocation();
//
// 	FRotator LookAtRotation=(PlayerLocation-EnemyLocation).Rotation();
// 	if (UWidgetComponent* BarWidget=Cast<UWidgetComponent>(HPBar))
// 	{
// 		BarWidget->SetWorldRotation(FRotator(0.f, LookAtRotation.Yaw, 0.f));
// 	}
// }

float ABaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	if (AEnemyAIController* AIController=Cast<AEnemyAIController>(GetController()))
	{
		AIController->SetAIState(EAIState::Frozen);
	}
	//메시 유효 
	if (!GetMesh()) return 0.0f;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//둘다 유효
	if (AnimInstance&&HitMontage)
	{
		//피격 애니메이션 재생
		AnimInstance->Montage_Play(HitMontage);
		//몽타주 끝났을 때 이벤트 바인딩
		AnimInstance->OnMontageEnded.Clear();
		AnimInstance->OnMontageEnded.AddDynamic(this,&ABaseEnemy::OnMontageEnded);
	}
	//데미지 인식 이벤트 호출
	if (DamageCauser)
	{
		UAISense_Damage::ReportDamageEvent(
			GetWorld(),
			this,
			EventInstigator->GetPawn(),
			ActualDamage,
			GetActorLocation(),
			DamageCauser->GetActorLocation()
		);
	}
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

//Montage가 실행이 끝나면 호출되는 함수
void ABaseEnemy::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	//공격 몽타주는 끝나면 델리게이트에 Broadcast하여 종료 알림 (Task 작업고 관련)
	if (Montage==AttackMontage)
	{
		OnAttackEnd.Broadcast();
	}
	//피격 모션이 끝나면 상태 Attacking으로 변경
	if(Montage==HitMontage)
	{
		if (AEnemyAIController* EnemyController=Cast<AEnemyAIController>(GetController()))
		{
			EnemyController->SetAIState(EAIState::Attacking);
		}
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

