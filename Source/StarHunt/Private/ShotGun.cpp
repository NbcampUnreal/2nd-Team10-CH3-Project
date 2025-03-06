// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotGun.h"
#include "BaseBullet.h"

AShotGun::AShotGun()
{
    GunType = EGunType::ShotGun;
    PitchRecoil = -3.0f;
    MinYawRecoil = -1.0f;
    MaxYawRecoil = 1.0f;
    
    NumberOfPellets = 8;
    SpreadAngle = 10.0f;
    Damage = 35.0f;
    FireRate = 1.5f;
    ReloadTime = 1.5f;
    MaxAmmo = 5;
    CurrentAmmo = MaxAmmo;
}

void AShotGun::FireProgress()
{
    for (int32 i = 0; i < NumberOfPellets; i++)
    {
        ABaseBullet* SpawnedBullet = SpawnBullet();

        if (!SpawnedBullet) return;
        SpawnedBullet->SetBulletDamage(Damage / NumberOfPellets);
    }

    CurrentAmmo--;
    //UE_LOG(LogTemp, Warning, TEXT("Bullet : %d / %d"), CurrentAmmo, MaxAmmo);
}
