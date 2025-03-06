// Fill out your copyright notice in the Description page of Project Settings.
// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PatrolPath.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BaseEnemy.generated.h"

//전방 선언
enum class EMovementSpeed : uint8;
class UItemSpawnComponent;
class UWidgetComponent;
class UItemSpawnComponent;
//Delegate 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnd);

UCLASS()
class STARHUNT_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	
	//Power of Character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power")
	float Power;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Power")
	bool bIsDead;
	
	//Attack Montage
	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* AttackMontage;
	
	// Score
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;

	//Patrol Path
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path")
	APatrolPath* PatrolPath;
	
	// Max Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	// UWidgetComponent* HPBar;

	//사거리 관련 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float AttackRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float DefendRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTree;

	// Death handling function
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void OnDeath();

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Get Patrol Path
	UFUNCTION(BlueprintPure, Category = "Path")
	APatrolPath* GetPatrolPath() const;

	// Get health variables
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetMaxHealth() const;
	// Healing
	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(const float Amount);
	// Damage handling function
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual void SetMovementSpeed(const EMovementSpeed Speed);

	//Get BehaviorTree
	UFUNCTION(BlueprintPure, Category = "AI")
	UBehaviorTree* GetBehaviorTree() const;

	UFUNCTION(BlueprintCallable, Category = "AI")
	virtual void Attack();

	UPROPERTY(BlueprintAssignable, Category="Attack")
	FOnAttackEnd OnAttackEnd;

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	float GetAttackRadius() const;
	UFUNCTION()
	float GetDefendRadius() const;
	UFUNCTION()
	void DelayedDestroy();
	// UFUNCTION()
	// void UpdateHPBar() const;

	// virtual void Tick(float DeltaSeconds) override;

	//아이템
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UItemSpawnComponent* ItemSpawnComponent;
};

