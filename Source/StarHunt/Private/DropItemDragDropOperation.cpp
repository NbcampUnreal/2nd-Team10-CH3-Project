// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItemDragDropOperation.h"
#include "ItemBlueprintFunctionLibrary.h"
#include "DropItemActor.h"

bool UDropItemDragDropOperation::DropInventory(int32 DropSlotIndex, EInventoryType DropSlotType)
{
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		if (DropItem && DropItem->IsValid())
		{
			if (ItemSubsystem->AddItem(*(*DropItem).Get()))
			{
				*(*DropItem).Get() = nullptr;
				if (DropItemSlot)
				{
					DropItemSlot->UpdateSlot();
				}
				if (DropItemActor)
				{
					DropItemActor->ItemsUpdate();
				}
				return true;
			}

		}
	}
	return false;
}

bool UDropItemDragDropOperation::DropEquipment(int32 DropSlotIndex, EInventoryType DropSlotType)
{
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		if (DropItem && DropItem->IsValid())
		{
			if (ItemSubsystem->AddGunEquipmentByDropGun(DropSlotIndex, *(*DropItem).Get()))
			{
				*(*DropItem).Get() = nullptr;
				if (DropItemSlot)
				{
					DropItemSlot->UpdateSlot();
				}
				DropItemActor->ItemsUpdate();
				return true;
			}

		}
	}

	return false;
}