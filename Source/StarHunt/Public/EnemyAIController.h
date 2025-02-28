// Fill out your copyright notice in the Description page of Project Settings.
// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIEnum.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
//전방 선언
class UAIPerceptionComponent;
class UAISense;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UAISenseConfig_Damage;
struct FAIStimulus;

UCLASS()
class STARHUNT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()


protected:
	AEnemyAIController();
	//Blackboard의 키 이름
	const FName StateKeyName="State";
	const FName AttackTargetKeyName="AttackTarget";
	const FName PointOfInterestKeyName="PointOfInterest";

	//AI perception 컴포넌트
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI|Perception", meta=(AllowPrivateAccess="true"))
	UAIPerceptionComponent* PerceptionComp;

	//시각 감지
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI|Perception")
	UAISenseConfig_Sight* SightSense;
	//청각 감지
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI|Perception")
	UAISenseConfig_Hearing* HearingSense;
	//피해 감지
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI|Perception")
	UAISenseConfig_Damage* DamageSense;
	
	UPROPERTY(VisibleAnywhere,Category="AI")
	EAIState CurrentState;
	
public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	//현재 상태 반환 함수
	UFUNCTION(BlueprintPure,Category="AI")
	EAIState GetCurrentState() const;
	//AI 상태 변경 함수
	UFUNCTION(BlueprintCallable,Category="AI")
	void SetAIState(EAIState NewState);
	//BlackBoard에 AttackTarget 등록
	UFUNCTION(BlueprintCallable,Category="AI")
	void SetAttackTarget(AActor* AttackTarget);
	//소리가 난 곳을 흥미가 가는 지점으로 등록
	UFUNCTION(BlueprintCallable,Category="AI")
	void SetPointOfInterest(const FVector PointOfInterest);

	//특정 액터 감지 가능한지 판별
	UFUNCTION(BlueprintCallable,Category="AI")
	bool CanSenseActor(AActor* TargetActor, TSubclassOf<UAISense> SenseClass, FAIStimulus& OutStimulus);

	UFUNCTION(BlueprintCallable,Category="AI")
	void HandleSenseSight(AActor* TargetActor);

	UFUNCTION(BlueprintCallable,Category="AI")
	void HandleSenseHearing(const FVector& SoundLocation);

	UFUNCTION(BlueprintCallable,Category="AI")
	void HandleSenseDamage(AActor* DamageCauser);
	
	//Perception 업데이트 이벤트
	UFUNCTION(BlueprintCallable,Category="AI")
	void OnPerceptionUpdated(const TArray<AActor*>&UpdatedActors);
};




