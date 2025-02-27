// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDB.h"


FBaseItemStateRow* UItemDB::GetBaseItemStateRow(const FString& ItemId)
{
	if (BaseItemDataTable)
	{
		return BaseItemDataTable->FindRow<FBaseItemStateRow>(FName(ItemId), TEXT("BaseItemDataTable"));
	}
	return nullptr;
}

FHealingItemStateRow* UItemDB::GetHealingItemStateRow(const FString& ItemId)
{
	if (HealingItemDataTable)
	{
		return HealingItemDataTable->FindRow<FHealingItemStateRow>(FName(ItemId), TEXT("HealingItemDataTable"));
	}
	return nullptr;
}

FGunItemStateRow* UItemDB::GetGunItemStateRow(const FString& ItemId)
{
	if (GunItemDataTable)
	{
		return GunItemDataTable->FindRow<FGunItemStateRow>(FName(ItemId), TEXT("GunItemDataTable"));
	}
	return nullptr;
}

FGunFixtureItemStateRow* UItemDB::GetGunFixtureItemStateRow(const FString& ItemId)
{
	if (GunFixtureItemDataTable)
	{
		return GunFixtureItemDataTable->FindRow<FGunFixtureItemStateRow>(FName(ItemId), TEXT("GunFixtureItemDataTable"));
	}
	return nullptr;
}
