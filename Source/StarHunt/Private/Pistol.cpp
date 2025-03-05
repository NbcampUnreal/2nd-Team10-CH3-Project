// Fill out your copyright notice in the Description page of Project Settings.


#include "Pistol.h"

APistol::APistol()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> EquipAnimMontage(TEXT("/Game/Characters/RetargetWraithAnim/RTA_MM_Pistol_Equip_Montage.RTA_MM_Pistol_Equip_Montage"));
	EquipMontage = EquipAnimMontage.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireAnimMontage(TEXT("/Game/Characters/RetargetWraithAnim/RTA_AM_MM_Pistol_Fire.RTA_AM_MM_Pistol_Fire"));
	FireMontage = FireAnimMontage.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ReloadAnimMontage(TEXT("/Game/Characters/RetargetWraithAnim/RTA_AM_MM_Pistol_Reload.RTA_AM_MM_Pistol_Reload"));
	ReloadMontage = ReloadAnimMontage.Object;

	GunType = EGunType::Pistol;
	Damage = 10.0f;
	FireRate = 0.5f;
	ReloadTime = 1.0f;
	MaxAmmo = 30;
	CurrentAmmo = MaxAmmo;
}
