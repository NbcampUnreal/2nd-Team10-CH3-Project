// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingItemActivate.h"

UHealingItemActivate::UHealingItemActivate()
	: HealingAmount(0.0f)
{
}
UHealingItemActivate::UHealingItemActivate(FHealingItemStateRow& HealingItemStateRow)
{
	HealingAmount = HealingItemStateRow.HealingAmount;
}

void UHealingItemActivate::ActivateItem(AActor* Activator)
{
	UE_LOG(LogTemp, Warning, TEXT("ActivateHealingItem : %f"), HealingAmount);
}

