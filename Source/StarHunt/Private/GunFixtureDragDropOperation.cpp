// Fill out your copyright notice in the Description page of Project Settings.


#include "GunFixtureDragDropOperation.h"
#include "ItemBlueprintFunctionLibrary.h"

bool UGunFixtureDragDropOperation::DropInventory(int32 DropSlotIndex, EInventoryType DropSlotType)
{
	if (SlotType == DropSlotType)
	{
		if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
		{
			if (ItemSubsystem->RemoveGunFixtureEquipment(SlotIndex, GunFixtureType))
			{
				return true;
			}
		}
	}
	return false;
}

bool UGunFixtureDragDropOperation::DropEquipment(int32 DropSlotIndex, EInventoryType DropSlotType)
{
	if (DropSlotType == EInventoryType::Gun || DropSlotType == EInventoryType::GunFixture)
	{
		if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
		{
			if (ItemSubsystem->SwapGunFixtureEquipment(SlotIndex, DropSlotIndex, GunFixtureType))
			{
				return true;
			}
		}
	}
	return false;
}

