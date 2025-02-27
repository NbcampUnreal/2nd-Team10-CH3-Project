// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotGun.h"
#include "BaseBullet.h"

AShotGun::AShotGun()
{
    NumberOfBullets = 8;
    SpreadAngle = 10.0f;
    Damage = 35.0f;
    FireRate = 1.5f;
    ReloadTime = 1.5f;
    MaxAmmo = 5;
    CurrentAmmo = MaxAmmo;
}

void AShotGun::FireProgress()
{
    FVector SpawnLocation = BulletSpawnLocation->GetComponentLocation();
    FRotator BaseRotation = GetActorRotation();

    for (int32 i = 0; i < NumberOfBullets; i++)
    {
        float RandomYaw = FMath::FRandRange(-SpreadAngle / 2.0f, SpreadAngle / 2.0f);
        float RandomPitch = FMath::FRandRange(-SpreadAngle / 2.0f, SpreadAngle / 2.0f);

        FRotator BulletRotation = BaseRotation;
        BulletRotation.Yaw += RandomYaw;
        BulletRotation.Pitch += RandomPitch;

        ABaseBullet* SpawnedBullet = GetWorld()->SpawnActor<ABaseBullet>(BulletClass, SpawnLocation, BulletRotation);

        if (!SpawnedBullet) return;
        
        SpawnedBullet->SetBulletDamage(Damage / NumberOfBullets);
        
    }

    CurrentAmmo--;
    //UE_LOG(LogTemp, Warning, TEXT("Bullet : %d / %d"), CurrentAmmo, MaxAmmo);
}
