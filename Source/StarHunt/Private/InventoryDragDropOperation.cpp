// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryDragDropOperation.h"
#include "ItemBlueprintFunctionLibrary.h"

bool UInventoryDragDropOperation::DropInventory(int32 DropSlotIndex, const FString& DropSlotType)
{

	if (DropSlotType  == SlotType)
	{
		if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
		{
			if (ItemSubsystem->SwapItem(SlotType, SlotIndex, DropSlotIndex))
			{
				return true;
			}
		}

	}
	return false;
}

bool UInventoryDragDropOperation::DropEquipment(int32 DropSlotIndex, const FString& DropSlotType)
{
	if (SlotType == DropSlotType)
	{
		if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
		{
			if (ItemSubsystem->AddGunEquipment(DropSlotIndex, SlotIndex))
			{
				ItemSubsystem->OnEquipmentChange.Broadcast(DropSlotIndex);
				ItemSubsystem->RemoveItem(SlotType, SlotIndex);
				ItemSubsystem->OnInventoryChange.Broadcast(SlotIndex);

				return true;
			}
		}
	}
	return false;
}
