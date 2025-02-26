// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunInterface.h"
#include "RarityInterface.h"
#include "BaseGun.generated.h"

class USceneComponent;
class ABaseBullet;

UCLASS(Abstract)
class STARHUNT_API ABaseGun : public AActor, public IGunInterface, public IRarityInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseGun();

public:
	// 발사
	UFUNCTION(BlueprintCallable)
	virtual void Fire() override;

	// 발사 중지
	UFUNCTION(BlueprintCallable)
	virtual void StopFire() override;

	// 장전
	UFUNCTION(BlueprintCallable)
	virtual void Reload() override final;

protected:
	// 공격 가능 여부 체크
	virtual bool CanAttack();

	// 발사 속도 타이머 초기화
	void ResetFireTimer();

	// 발사 과정
	virtual void FireProgress();

protected:
	// 루트 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GunIComponent")
	USceneComponent* Scene;
	// 스태틱 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GunIComponent")
	UStaticMeshComponent* StaticMesh;
	// 총알 스포너
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GunIComponent")
	USceneComponent* BulletSpawnLocation;
	// 총알 클래스
	// 에디터에서 총알 종류 선택 가능
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|Bullet")
	TSubclassOf<ABaseBullet> BulletClass;

	// 데미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GunIStatus")
	float Damage;

	// 발사 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GunIStatus")
	float FireRate;

	// 재장전 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GunIStatus")
	float ReloadTime;

	// 최대 탄창 수
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GunIStatus")
	int32 MaxAmmo;

	// 현재 탄창 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunIStatus")
	int32 CurrentAmmo;

	// 등급
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunIStatus")
	ERarity Rarity;

	// 발사 속도 타이머
	FTimerHandle FireRateTimerHandle;

	// 타이머 관련 변수
	bool bIsFiring;

	/*
	// 허용할 부착물 목록
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunIAttachments")
	TMap<enum class EAttachmentType, bool> AvailableSlots;

	// 장착된 부착물 목록
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunIAttachments")
	TMap<EAttachmentType, class AAttachment*> AttachedAttachments;
	*/
};