// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGameInstance.h"
#include "ItemSubsystem.h"

UShooterGameInstance::UShooterGameInstance()
{
	TotalScore = 0;
	TotalPlayTime = 0.f;
}

void UShooterGameInstance::Init()
{
	Super::Init();

	UItemSubsystem* ItemSubsystem = GetSubsystem<UItemSubsystem>();
	ItemSubsystem->SetItemDb(ItemDBClass);
}

void UShooterGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
}
