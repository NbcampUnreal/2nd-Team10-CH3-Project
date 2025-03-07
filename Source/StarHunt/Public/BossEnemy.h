// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"

#include "BossEnemy.generated.h"

class ABaseBullet;
/**
 * 
 */
UCLASS()
class STARHUNT_API ABossEnemy : public ABaseEnemy
{
	GENERATED_BODY()

public:
	ABossEnemy();
	virtual void SetMovementSpeed(const EMovementSpeed Speed) override;
	void BeginPlay() override;
	virtual void Attack() override;

	UFUNCTION(BlueprintCallable, Category="Attack")
	void Fire();

	UFUNCTION()
	void SpecialFire(FName NotifyName, const FBranchingPointNotifyPayload& Payload);

protected:
	// 에디터에서 총알 종류 선택 가능
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun|Bullet")
	TSubclassOf<class ABaseBullet> BulletClass;

	// 에디터에서 총알 종류 선택 가능
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun|Bullet")
	TSubclassOf<class ABaseBullet> SpecialBulletClass;
};
