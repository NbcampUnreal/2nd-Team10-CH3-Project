// Fill out your copyright notice in the Description page of Project Settings.


#include "SubMachineGun.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "BaseBullet.h"

ASubMachineGun::ASubMachineGun()
{
	bIsFiring = false;

	Damage = 5.0f;
	FireRate = 0.1f;
	ReloadTime = 1.0f;
	MaxAmmo = 100;
	CurrentAmmo = MaxAmmo;
}

void ASubMachineGun::Fire()
{
	// 이미 발사 중이라면 추가 호출 무시
	if (bIsFiring)
		return;

	bIsFiring = true;

	FireProgress();

	// 마우스를 누르고 있는 동안에는 계속 FireProgress 함수가 실행
	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &ASubMachineGun::FireProgress, FireRate, true);
}

void ASubMachineGun::StopFire()
{
	bIsFiring = false;
	GetWorld()->GetTimerManager().ClearTimer(FireRateTimerHandle);
}