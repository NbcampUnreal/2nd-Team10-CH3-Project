// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingItemActivate.h"
#include "PlayerCharacter.h"
#include "ItemBlueprintFunctionLibrary.h"

UHealingItemActivate::UHealingItemActivate()
	: HealingAmount(0.0f)
{

}

void UHealingItemActivate::ActivateItem(AActor* Activator)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Activator))
	{
		PlayerCharacter->AddHealth(HealingAmount);
	}
}

void UHealingItemActivate::SetStatus(const FString& ItemID)
{
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		if (FHealingItemStateRow* HealingItemStateRow = ItemSubsystem->GetHealingItemStateRow(ItemID))
		{
			HealingAmount = HealingItemStateRow->HealingAmount;
		}
	}
}

