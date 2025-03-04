// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseBullet.generated.h"

UCLASS(Abstract)
class STARHUNT_API ABaseBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseBullet();

protected:
	// 루트 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet|Component")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet|Component")
	class USphereComponent* Collision;
	// 스태틱 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet|Component")
	class UStaticMeshComponent* StaticMesh;
	// 발사체
    UPROPERTY(VisibleAnywhere, Category = "Bullet|Component")
	class UProjectileMovementComponent* ProjectileMovement;

	// 총알 데미지
    float BulletDamage;

public:
	UFUNCTION(BlueprintCallable)
    void SetBulletDamage(float Damage);
	UFUNCTION(BlueprintCallable)
	float GetBulletDamage() const;

protected:
	// 총알 충돌 이벤트
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
