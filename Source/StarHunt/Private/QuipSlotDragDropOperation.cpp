// Fill out your copyright notice in the Description page of Project Settings.


#include "QuipSlotDragDropOperation.h"
#include "ItemBlueprintFunctionLibrary.h"

bool UQuipSlotDragDropOperation::DropInventory(int32 DropSlotIndex, EInventoryType DropSlotType)
{
	if (UItemSubsystem* ItemSubSystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		ItemSubSystem->SetQuickSlotEmpty(SlotIndex);
		ItemSubSystem->OnQuickSlotChange.Broadcast(SlotIndex);
	}
	return false;
}

// 퀵슬롯 여러개일 경우 swap형식으로 구현할것
bool UQuipSlotDragDropOperation::DropQuickSlot(int32 DropSlotIndex, EInventoryType DropSlotType)
{
	return false;
}

// 퀵슬롯이 장비창에 드래그앤 드랍했을경우는 반응하지않음
bool UQuipSlotDragDropOperation::DropEquipment(int32 DropSlotIndex, EInventoryType DropSlotType)
{
	return false;
}

