
// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGun.h"
#include "BaseBullet.h"
#include "Kismet/GameplayStatics.h"
#include "AimingSystem.h"

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

	SpreadAngle = 0.0f;
	Damage = 1.0f;
	FireRate = 1.0f;
	ReloadTime = 1.0f;
	MaxAmmo = 1;
	CurrentAmmo = MaxAmmo;
	bIsFiring = false;
	BulletClass = ABaseBullet::StaticClass();
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
	CurrentAmmo = MaxAmmo;
}

void ABaseGun::SetAbility(FGunItemStateRow* GunItemStateRow)
{
	if (GunItemStateRow)
	{
		Damage = GunItemStateRow->DamageAmount;
		FireRate = GunItemStateRow->FireRate;
		MaxAmmo =GunItemStateRow->MaxAmmo;
		GunType =GunItemStateRow->GunType;
	}
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
		UE_LOG(LogTemp, Warning, TEXT("No Bullet"));
		return false;
	}
	if (bIsFiring)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cool Time"));
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

	// 총알 소환
	ABaseBullet* SpawnedBullet = SpawnBullet();

	if (!SpawnedBullet) return;

	// 총알 데미지 설정
	SpawnedBullet->SetBulletDamage(Damage);

	// 총알 감소
	CurrentAmmo--;
	//UE_LOG(LogTemp, Warning, TEXT("Bullet : %d / %d"), CurrentAmmo, MaxAmmo);
}

ABaseBullet* ABaseGun::SpawnBullet()
{
	// 총알 위치, 방향 설정
	FVector SpawnLocation = BulletSpawnLocation->GetComponentLocation();
	FRotator SpawnRotation = CaculateAimRotator();

	float Angle = SpreadAngle / 2.0f;
	SpawnRotation.Yaw += FMath::FRandRange(-Angle, Angle);
	SpawnRotation.Pitch += FMath::FRandRange(-Angle, Angle);
	ABaseBullet* SpawnedBullet = GetWorld()->SpawnActor<ABaseBullet>(BulletClass, SpawnLocation, SpawnRotation);

	if (SpawnedBullet)
	{
		// 총알의 소유자를 총으로 설정
		SpawnedBullet->SetOwner(this);

		return SpawnedBullet;
	}
	else
	{
		return nullptr;
	}
}

FRotator ABaseGun::CaculateAimRotator()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	if (PlayerController)
	{
		TObjectPtr<UAimingSystem> AimingSystem = NewObject<UAimingSystem>();
		FVector SpawnLocation = BulletSpawnLocation->GetComponentLocation();

		return AimingSystem->GetTargetRotation(PlayerController, SpawnLocation);
	}
	else
	{
		return GetActorRotation();
	}
}
