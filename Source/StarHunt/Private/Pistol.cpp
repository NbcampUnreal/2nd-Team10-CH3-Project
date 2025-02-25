// Fill out your copyright notice in the Description page of Project Settings.


#include "Pistol.h"

APistol::APistol()
{
	Damage = 10.0f;
	FireRate = 0.5f;
	ReloadTime = 1.0f;
	MaxAmmo = 30;
	CurrentAmmo = MaxAmmo;
}
