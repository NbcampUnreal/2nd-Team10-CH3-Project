
// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGun.h"
#include "BaseBullet.h"

ABaseGun::ABaseGun()
{
	// 씬 컴포넌트 생성
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	// 스태틱 메시 생성
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GunMesh->SetupAttachment(Scene);

	BulletSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawnLocation"));
	BulletSpawnLocation->SetupAttachment(Scene);

	GunFixtureComponent = CreateDefaultSubobject<UGunFixtureComponent>(TEXT("GunFixtureComponent"));

	Damage = 1.0f;
	FireRate = 1.0f;
	ReloadTime = 1.0f;
	MaxAmmo = 1;
	CurrentAmmo = MaxAmmo;
	bIsFiring = false;
}

void ABaseGun::Fire()
{
	if (!CanAttack()) return;

	FireProgress();

	// 타이머로 발사 속도 제한
	bIsFiring = true;
	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &ABaseGun::ResetFireTimer, FireRate, false);
}

void ABaseGun::StopFire()
{
}

void ABaseGun::Reload()
{
	int32 SumMaxAmmo = MaxAmmo;
	if (GunFixtureComponent)
	{
		if (FGunFixtureItemStateRow* SumGunFixtureItemStateRow = GunFixtureComponent->GetFixtursStatus())
		{
			MaxAmmo += SumGunFixtureItemStateRow->MaxAmmoAmount;
		}
	}

	CurrentAmmo = MaxAmmo;
}


void ABaseGun::StartItemSubsystem(int32 EquipmentIndex, FGunItemStateRow* GunItemStateRow)
{
	GunFixtureComponent->StartItemSubsystem(EquipmentIndex);
	if (GunItemStateRow)
	{
		Damage = GunItemStateRow->DamageAmount;
		FireRate = GunItemStateRow->FireRate;
		MaxAmmo = GunItemStateRow->MaxAmmo;
		GunType = GunItemStateRow->GunType;
	}
}

void ABaseGun::EndItemSubsystem()
{
	GunFixtureComponent->EndItemSubsystem();

}


bool ABaseGun::CanAttack()
{
	if (!BulletClass || !BulletClass->IsChildOf<ABaseBullet>())
	{
		UE_LOG(LogTemp, Error, TEXT("Need to select a Bullet Class on Blueprint"));
		return false;
	}
	if (CurrentAmmo <= 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("No Bullet"));
		return false;
	}
	if (bIsFiring)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Cool Time"));
		return false;
	}

	return true;
}

void ABaseGun::ResetFireTimer()
{
	bIsFiring = false;
}

void ABaseGun::FireProgress()
{
	if (CurrentAmmo <= 0) return;

	// 탄환 생성
	FVector SpawnLocation = BulletSpawnLocation->GetComponentLocation();
	FRotator SpawnRotation = GetActorRotation();
	ABaseBullet* SpawnedBullet = GetWorld()->SpawnActor<ABaseBullet>(BulletClass, SpawnLocation, SpawnRotation);

	if (!SpawnedBullet) return;

	// 총알의 소유자를 총으로 설정
	SpawnedBullet->SetOwner(this);
	// 총알에 데미지 전달
	float SumDamage = Damage;
	if (GunFixtureComponent)
	{
		if (FGunFixtureItemStateRow* SumGunFixtureItemStateRow = GunFixtureComponent->GetFixtursStatus())
		{
			SumDamage += SumGunFixtureItemStateRow->DamageAmount;
		}
	}
	SpawnedBullet->SetBulletDamage(SumDamage);

	// 총알 감소
	CurrentAmmo--;
	//UE_LOG(LogTemp, Warning, TEXT("Bullet : %d / %d"), CurrentAmmo, MaxAmmo);
}

