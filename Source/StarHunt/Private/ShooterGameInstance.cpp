// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGameInstance.h"
#include "ItemSubsystem.h"

UShooterGameInstance::UShooterGameInstance()
{
	TotalScore = 0;
	TotalPlayTimeMinute = 0;
	TotalPlayTimeSec = 0;
	CurrentWave = 0;
}

void UShooterGameInstance::Init()
{
	Super::Init();

	if (UItemSubsystem* ItemSubsystem = GetSubsystem<UItemSubsystem>())
	{
		ItemSubsystem->SetItemDb(ItemDBClass);

		//test 용
		ItemSubsystem->AddGunEquipmentByDropGun(0, FString("PistolGunCommon1"));
		ItemSubsystem->AddGunEquipmentByDropGun(1, FString("RifleGunCommon1"));
		ItemSubsystem->AddGunEquipmentByDropGun(2, FString("ShotGunGunCommon1"));
	}
}

void UShooterGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	
}
