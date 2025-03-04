// Fill out your copyright notice in the Description page of Project Settings.


#include "Pistol.h"

APistol::APistol()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AnimMontage(TEXT("/Game/Characters/RetargetWraithAnim/RTA_MM_Pistol_Equip_Montage.RTA_MM_Pistol_Equip_Montage"));
	EquipMontage = AnimMontage.Object;
	
	Damage = 10.0f;
	FireRate = 0.5f;
	ReloadTime = 1.0f;
	MaxAmmo = 30;
	CurrentAmmo = MaxAmmo;
}
