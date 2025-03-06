
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

	GunFixtureComponent = CreateDefaultSubobject<UGunFixtureComponent>(TEXT("GunFixtureComponent"));

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
	int32 SumMaxAmmo = MaxAmmo;
	if (GunFixtureComponent)
	{
		if (FGunFixtureItemStateRow* SumGunFixtureItemStateRow = GunFixtureComponent->GetFixtursStatus())
		{
			MaxAmmo += SumGunFixtureItemStateRow->MaxAmmoAmount;
		}
	}

	CurrentAmmo = MaxAmmo;

	UE_LOG(LogTemp, Warning, TEXT("Current Ammo : %d"), CurrentAmmo);
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

EGunType ABaseGun::GetGunType() const
{
	return GunType;
}

UAnimMontage* ABaseGun::GetEquipMontage() const
{
	return EquipMontage;
}

UAnimMontage* ABaseGun::GetUnEquipMontage() const
{
	return UnEquipMontage;
}

UAnimMontage* ABaseGun::GetFireMontage() const
{
	return FireMontage;
}

UAnimMontage* ABaseGun::GetReloadMontage() const
{
	return ReloadMontage;
}

UAnimationAsset* ABaseGun::GetGunFireAnimation() const
{
	return GunFireAnimation;
}

UAnimationAsset* ABaseGun::GetGunReloadAnimation() const
{
	return GunReloadAnimation;
}

float ABaseGun::GetPitchRecoil() const
{
	return PitchRecoil;
}

float ABaseGun::GetYawRecoil() const
{
	float YawRecoil = FMath::RandRange(MinYawRecoil, MaxYawRecoil);
	return YawRecoil;
}

void ABaseGun::PlayFireAnim()
{
	GunMesh->PlayAnimation(GunFireAnimation, false);
}

void ABaseGun::PlayReloadAnim()
{
	GunMesh->PlayAnimation(GunReloadAnimation, false);
}

void ABaseGun::ResetFireTimer()
{
	bIsFiring = false;
}

void ABaseGun::FireProgress()
{
	if (CurrentAmmo <= 0) return;

	PlayFireAnim();
	
	// 총알 소환
	ABaseBullet* SpawnedBullet = SpawnBullet();

	if (!SpawnedBullet) return;


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

ABaseBullet* ABaseGun::SpawnBullet()
{
	// 총알 위치, 방향 설정
	FVector SpawnLocation = BulletSpawnLocation->GetComponentLocation();
	FRotator SpawnRotation = CaculateAimRotator();

	float Angle = SpreadAngle / 2.0f;
	SpawnRotation.Yaw += FMath::FRandRange(-Angle, Angle);
	SpawnRotation.Pitch += FMath::FRandRange(-Angle, Angle);
	FActorSpawnParameters SpawnParameter;
	SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	auto* SpawnClass = BulletClass.Get();
	AActor* SpawnedBullet = GetWorld()->SpawnActor<AActor>(SpawnClass, SpawnLocation, SpawnRotation, SpawnParameter);

	if (SpawnedBullet)
	{
		// 총알의 소유자를 총으로 설정
		SpawnedBullet->SetOwner(this);

		return Cast<ABaseBullet>(SpawnedBullet);
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
