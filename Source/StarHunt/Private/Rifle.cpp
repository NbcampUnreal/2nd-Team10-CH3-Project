// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "BaseBullet.h"

ARifle::ARifle()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> EquipAnimMontage(TEXT("/Game/Characters/RetargetWraithAnim/RTA_AM_MM_Rifle_Equip.RTA_AM_MM_Rifle_Equip"));
	EquipMontage = EquipAnimMontage.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireAnimMontage(TEXT("/Game/Characters/RetargetWraithAnim/RTA_AM_MM_Rifle_Fire.RTA_AM_MM_Rifle_Fire"));
	FireMontage = FireAnimMontage.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ReloadAnimMontage(TEXT("/Game/Characters/RetargetWraithAnim/RTA_AM_MM_Rifle_Reload.RTA_AM_MM_Rifle_Reload"));
	ReloadMontage = ReloadAnimMontage.Object;
	
	bIsFiring = false;

	GunType = EGunType::Rifle;
	Damage = 5.0f;
	FireRate = 0.1f;
	ReloadTime = 1.0f;
	MaxAmmo = 100;
	CurrentAmmo = MaxAmmo;
}

void ARifle::Fire()
{
	// 이미 발사 중이라면 추가 호출 무시
	if (bIsFiring) return;

	bIsFiring = true;

	FireProgress();

	// 마우스를 누르고 있는 동안에는 계속 FireProgress 함수가 실행
	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &ARifle::FireProgress, FireRate, true);
}

void ARifle::StopFire()
{
	bIsFiring = false;
	GetWorld()->GetTimerManager().ClearTimer(FireRateTimerHandle);
}