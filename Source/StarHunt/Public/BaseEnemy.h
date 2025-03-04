// Fill out your copyright notice in the Description page of Project Settings.
// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PatrolPath.h"
#include "BaseEnemy.generated.h"

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
	// Get health variables
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetMaxHealth() const;

	// Healing
	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(const float Amount);

	// Death handling function
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void OnDeath();

	// Damage handling function
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Get Patrol Path
	UFUNCTION(BlueprintPure, Category = "Path")
	APatrolPath* GetPatrolPath() const;
};
