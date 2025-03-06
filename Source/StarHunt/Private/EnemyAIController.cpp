// Fill out your copyright notice in the Description page of Project Settings.
// Copyright Epic Games, Inc. All Rights Reserved.

#include "EnemyAIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BaseEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemyAIController::AEnemyAIController()
{
	//Perception 컴포넌트 생성 
	PerceptionComp=CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));

	//시각 감지 설정
	SightSense=CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightSense"));
	SightSense->SightRadius=800.0f;//시야 거리
	SightSense->LoseSightRadius=1200.0f;//대상 상실 범위
	SightSense->PeripheralVisionAngleDegrees=60.0f;//시야 각 (한쪽 눈)
	SightSense->SetMaxAge(5.0f);//정보 유지 시간
	//피아식별 관련 옵션
	SightSense->DetectionByAffiliation.bDetectEnemies=true;
	SightSense->DetectionByAffiliation.bDetectNeutrals=true;
	SightSense->DetectionByAffiliation.bDetectFriendlies=true;
	
	//청각 감지 설정
	HearingSense=CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingSense"));
	HearingSense->HearingRange=2000.0f;//청각 반경
	HearingSense->SetMaxAge(5.0f);//정보 유지 시간
	//피아식별 관련 옵션
	HearingSense->DetectionByAffiliation.bDetectEnemies=true;
	HearingSense->DetectionByAffiliation.bDetectNeutrals=true;
	HearingSense->DetectionByAffiliation.bDetectFriendlies=true;
	
	//피해 감지 설정
	DamageSense=CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageSense"));
	DamageSense->SetMaxAge(5.0f);

	//Perception Component에 Config 추가
	PerceptionComp->ConfigureSense(*SightSense);
	PerceptionComp->ConfigureSense(*HearingSense);
	PerceptionComp->ConfigureSense(*DamageSense);

	//Sense 우선순위 설정(시각 우선)
	PerceptionComp->SetDominantSense(UAISense_Sight::StaticClass());

	//OnPerceptionUpdated에 커스텀 이벤트 바인딩
	PerceptionComp->OnPerceptionUpdated.AddDynamic(this,&AEnemyAIController::OnPerceptionUpdated);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	// //애니메이션의 원활한 적용을 위한 가속도 관련 옵션 설정 (NavMovement나 AIController를 통한 이동 명령에 가속도를 적용)
	// if (ACharacter* Character1 = Cast<ACharacter>(InPawn))
	// {
	// 	if (UCharacterMovementComponent* MovementComp = Character1->GetCharacterMovement())
	// 	{
	// 		MovementComp->bRequestedMoveUseAcceleration=true;
	// 	}
	// }

	// 초기 상태 Passive 설정 및 블랙보드 키 할당(공격 및 회피 거리)
	if (ABaseEnemy* Enemy=Cast<ABaseEnemy>(InPawn))
	{
		if (UBehaviorTree* BT=Enemy->GetBehaviorTree())
		{
			RunBehaviorTree(BT);
			SetAIState(EAIState::Passive);
			//이상적인 공격, 수비 범위 캐릭터에서 받아와 블랙보드에 할당
			if (UBlackboardComponent* BB=GetBlackboardComponent())
			{
				BB->SetValueAsFloat(AttackRadiusKeyName,Enemy->GetAttackRadius());
				BB->SetValueAsFloat(DefendRadiusKeyName,Enemy->GetDefendRadius());
			}
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
		Target=AttackTarget;
	}
}

AActor* AEnemyAIController::GetAttackTarget() const
{
	return Target;
}
// 감지된 지점이나 이동 지점 해당 벡터로 처리
void AEnemyAIController::SetPointOfInterest(const FVector PointOfInterest)
{
	if (UBlackboardComponent* BB=GetBlackboardComponent())
	{
		BB->SetValueAsVector(PointOfInterestKeyName, PointOfInterest);
	}
}


void AEnemyAIController::HandleSenseSight(AActor* TargetActor)
{
	//감지된 액터가 유효한지 검사
	if (!TargetActor)
	{
		return;
	}
	//현재 상태가 공격 중이라면 그대로 유지
	if (CurrentState==EAIState::Attacking)
	{
		return;
	}
	//감지된 액터가 플레이어인지 확인
	ACharacter* PlayerCharacter=UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	if (TargetActor==PlayerCharacter)
	{
		// Passive, Investigating State라면
		if (CurrentState==EAIState::Passive || CurrentState==EAIState::Investigating)
		{
			//State를 Attacking으로 변경
			SetAttackTarget(PlayerCharacter);
			SetAIState(EAIState::Attacking);
		}
	}
}

void AEnemyAIController::HandleSenseHearing(const FVector& SoundLocation)
{
	if (CurrentState==EAIState::Attacking)
	{
		return;
	}
	//Passive, Investigating 일때만 상태 업데이트 (공격 중이라면 업데이트 X)
	if (CurrentState==EAIState::Passive || CurrentState==EAIState::Investigating)
	{
		//State를 Attacking으로 변경 및 흥미 지점 소리난 지점으로 설정
		SetPointOfInterest(SoundLocation);
		SetAIState(EAIState::Investigating);
	}
}

void AEnemyAIController::HandleSenseDamage(AActor* DamageCauser)
{
	SetAttackTarget(DamageCauser);
	if (CurrentState==EAIState::Passive || CurrentState==EAIState::Investigating)
	{
		SetAIState(EAIState::Attacking);
	}
}

void AEnemyAIController::OnPerceptionUpdated(const TArray<AActor*>&UpdatedActors)
{
	//감지된 액터들 모두 순회하며 검사 (시각,청각,피격)
	for (AActor* Actor : UpdatedActors)
	{
		//인식 관련 정보를 담을 변수
		FAIStimulus Stimulus;
		if (CanSenseActor(Actor,UAISense_Sight::StaticClass(),Stimulus))
		{
			HandleSenseSight(Actor);
		}
		else if (CanSenseActor(Actor,UAISense_Hearing::StaticClass(),Stimulus))
		{
			HandleSenseHearing(Actor->GetActorLocation());
		}
		else if (CanSenseActor(Actor,UAISense_Damage::StaticClass(),Stimulus))
		{
			HandleSenseDamage(Actor);
		}
	}
}

bool AEnemyAIController::CanSenseActor(AActor* TargetActor, TSubclassOf<UAISense> SenseClass, FAIStimulus& OutStimulus)
{
	//Perception 컴포넌트 유효 검사
	if (!TargetActor || !PerceptionComp)
	{
		return false;
	}

	//설정된 Perception 정보 받아올 변수
	FActorPerceptionBlueprintInfo PerceptionInfo;
	//TargetActor에 대해 감지된 정보 받아오기
	PerceptionComp->GetActorsPerception(TargetActor, PerceptionInfo);

	//받아온 정보 순회
	for (const FAIStimulus& Stimulus : PerceptionInfo.LastSensedStimuli)
	{
		//감지된 Sense의 클래스 특정
		TSubclassOf<UAISense> DetectedSense=UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus);
		//감지되었는지 검사하려던 감각과 동일하다면 인식된 정보 매개변수에 담아 true 반환
		if (DetectedSense==SenseClass && Stimulus.WasSuccessfullySensed())
		{
			OutStimulus=Stimulus;
			return true;
		}
	}
	
	return false;
}
