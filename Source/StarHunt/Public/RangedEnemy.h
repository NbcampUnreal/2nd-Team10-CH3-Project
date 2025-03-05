// Fill out your copyright notice in the Description page of Project Settings.
// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "RangedEnemy.generated.h"

//전방선언
class ABaseBullet;
/**
 * 
 */
UCLASS()
class STARHUNT_API ARangedEnemy : public ABaseEnemy
{
	GENERATED_BODY()
public:
	ARangedEnemy();

	virtual void SetMovementSpeed(const EMovementSpeed Speed) override;

	virtual void Attack() override;

	UFUNCTION(BlueprintCallable, Category="Attack")
	void Fire();

protected:
	// 에디터에서 총알 종류 선택 가능
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun|Bullet")
	TSubclassOf<class ABaseBullet> BulletClass;
};
