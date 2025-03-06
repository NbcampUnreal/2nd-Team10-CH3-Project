// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "BaseBullet.h"

ARifle::ARifle()
{
	bIsFiring = false;

	GunType = EGunType::Rifle;
	PitchRecoil = -0.5f;
	MinYawRecoil = -0.25f;
	MaxYawRecoil = 0.25f;
	
	Damage = 5.0f;
	FireRate = 0.1f;
	ReloadTime = 1.0f;
	MaxAmmo = 100;
	CurrentAmmo = MaxAmmo;
}

void ARifle::StopFire()
{
	bIsFiring = false;
	GetWorld()->GetTimerManager().ClearTimer(FireRateTimerHandle);
}