// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunInterface.h"
#include "RarityInterface.h"
#include "ItemStateRow.h"
#include "GunFixtureComponent.h"

#include "BaseGun.generated.h"

UCLASS()
class STARHUNT_API ABaseGun : public AActor, public IGunInterface, public IRarityInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseGun();

public:
	// 발사 애니메이션 실행
	UFUNCTION(BlueprintCallable)
	virtual void PlayFireAnim();

	// 장전 애니메이션 실행
	UFUNCTION(BlueprintCallable)
	virtual void PlayReloadAnim();

	
	// 발사
	UFUNCTION(BlueprintCallable)
	virtual void Fire() override;

	// 발사 중지
	UFUNCTION(BlueprintCallable)
	virtual void StopFire() override;

	// 장전
	UFUNCTION(BlueprintCallable)
	virtual void Reload() override final;

	// 공격 가능 여부 체크
	virtual bool CanAttack();

	UFUNCTION()
	EGunType GetGunType() const;
	UFUNCTION()
	UAnimMontage* GetEquipMontage() const;
	UFUNCTION()
	UAnimMontage* GetUnEquipMontage() const;
	UFUNCTION()
	UAnimMontage* GetFireMontage() const;
	UFUNCTION()
	UAnimMontage* GetReloadMontage() const;
	UFUNCTION()
	UAnimationAsset* GetGunFireAnimation() const;
	UFUNCTION()
	UAnimationAsset* GetGunReloadAnimation() const;
	UFUNCTION()
	float GetPitchRecoil() const;
	UFUNCTION()
	float GetYawRecoil() const;
	UFUNCTION()
	bool GetIsReloading() const;
	UFUNCTION()
	bool IsMaxAmmo() const;
protected:
	

	// 발사 속도 타이머 초기화
	void ResetFireTimer();

	// 발사 과정
	virtual void FireProgress();

	// 총알 소환
	virtual class ABaseBullet* SpawnBullet();

	FRotator CaculateAimRotator();

protected:
	
	// 루트 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GunIComponent")
	USceneComponent* Scene;
	// 스태틱 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GunIComponent")
	USkeletalMeshComponent* GunMesh;
	// 총알 스포너
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GunIComponent")
	USceneComponent* BulletSpawnLocation;

	// 부착물
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gun|Component")
	UGunFixtureComponent* GunFixtureComponent;

	// 총알 클래스
	// 에디터에서 총알 종류 선택 가능
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|Bullet")
	TSubclassOf<class ABaseBullet> BulletClass;

	// 사격 오차 범위
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GunIStatus")
	float SpreadAngle;

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
	// 장전 타이머
	FTimerHandle ReloadTimerHandle;

	// 타이머 관련 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GunIStatus")
	bool bIsFiring;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GunIStatus")
	bool bIsReloading;

	//총 종류
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunIStatus")
	EGunType GunType;
	
	// 총 장착 PlayerAnimMontage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* EquipMontage;

	// 총 해제 PlayerAnimMontage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* UnEquipMontage;

	// 총 발사 PlayerAnimMontage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* FireMontage;

	// 총 장전 PlayerAnimMontage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* ReloadMontage;

	// 총 발사 애니메이션 에셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimationAsset* GunFireAnimation;

	// 총 장전 애니메이션 에셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimationAsset* GunReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float PitchRecoil;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float MinYawRecoil;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float MaxYawRecoil;

	//총 부속품 SubSystem연결
public:
	virtual void StartItemSubsystem(int32 EquipmentIndex, FGunItemStateRow* GunItemStateRow);
	virtual void EndItemSubsystem();
};