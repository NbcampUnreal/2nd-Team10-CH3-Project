// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentDragDropOperation.h"
#include "ItemSubsystem.h"
#include "ItemBlueprintFunctionLibrary.h"

bool UEquipmentDragDropOperation::DropInventory(int32 DropSlotIndex, EInventoryType DropSlotType)
{
	if (SlotType == DropSlotType)
	{
		if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
		{
			if (ItemSubsystem->RemoveGunEquipment(SlotIndex))
			{
				return true;
			}
		}
	}
	return false;
}

bool UEquipmentDragDropOperation::DropEquipment(int32 DropSlotIndex, EInventoryType DropSlotType)
{
	if (SlotType == DropSlotType)
	{
		if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
		{
			if (ItemSubsystem->SwapGunEquipment(DropSlotIndex, SlotIndex))
			{
				return true;
			}
		}
	}

	return false;
}

bool UEquipmentDragDropOperation::DropQuickSlot(int32 DropSlotIndex, EInventoryType DropSlotType)
{
	return false;
}
