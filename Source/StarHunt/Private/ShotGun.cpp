// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotGun.h"
#include "BaseBullet.h"

AShotGun::AShotGun()
{
    static ConstructorHelpers::FObjectFinder<UAnimMontage> EquipAnimMontage(TEXT("/Game/Characters/RetargetWraithAnim/RTA_AM_MM_Rifle_Equip.RTA_AM_MM_Rifle_Equip"));
    EquipMontage = EquipAnimMontage.Object;
    static ConstructorHelpers::FObjectFinder<UAnimMontage> FireAnimMontage(TEXT("/Game/Characters/RetargetWraithAnim/RTA_AM_MM_Shotgun_Fire.RTA_AM_MM_Shotgun_Fire"));
    FireMontage = FireAnimMontage.Object;
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ReloadAnimMontage(TEXT("/Game/Characters/RetargetWraithAnim/RTA_AM_MM_Shotgun_Reload.RTA_AM_MM_Shotgun_Reload"));
    ReloadMontage = ReloadAnimMontage.Object;
    
    GunType = EGunType::ShotGun;
    
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
        UE_LOG(LogTemp, Warning, TEXT("될까?"));

        if (!SpawnedBullet) return;
        UE_LOG(LogTemp, Warning, TEXT("진행?"));
        SpawnedBullet->SetBulletDamage(Damage / NumberOfPellets);
    }

    CurrentAmmo--;
    //UE_LOG(LogTemp, Warning, TEXT("Bullet : %d / %d"), CurrentAmmo, MaxAmmo);
}
