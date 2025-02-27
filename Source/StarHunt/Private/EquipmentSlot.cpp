// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSlot.h"
#include "EquipmentDragDropOperation.h"
#include "ItemBlueprintFunctionLibrary.h"

void UEquipmentSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		EquipmentChangeHandler = ItemSubsystem->OnEquipmentChange.AddUObject(this, &UEquipmentSlot::UpdateSlotAt);
	}
}

void UEquipmentSlot::NativeDestruct()
{
	Super::NativeDestruct();
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		ItemSubsystem->OnEquipmentChange.Remove(EquipmentChangeHandler);
	}
}

void UEquipmentSlot::UpdateSlot()
{
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		if (TSharedPtr<FString> ItemID = ItemSubsystem->GetEquipmentGunItemID(SlotIndex))
		{
			BaseItemStateRow = ItemSubsystem->GetBaseItemStateRow(*ItemID);

			UpdateUI();
			return;
		}
	}

	BaseItemStateRow = nullptr;
	UpdateUI();
}

void UEquipmentSlot::UpdateUI()
{
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		if (TSharedPtr<FString> ItemID = ItemSubsystem->GetEquipmentGunItemID(SlotIndex))
		{
			BaseItemStateRow = ItemSubsystem->GetBaseItemStateRow(*ItemID);
			if (UTexture2D* Texture2D = Cast<UTexture2D>(BaseItemStateRow->InventoryIcon))
			{
				if (ItemImage)
				{
					ItemImage->SetBrushFromTexture(Texture2D);
					return;
				}
			}
		}
		else if (ItemImage && DefaultImageTexture)
		{
			ItemImage->SetBrushFromTexture(DefaultImageTexture);
		}
	}
}

void UEquipmentSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (OutOperation == nullptr)
	{
		UEquipmentDragDropOperation* EquipmentDragDropOperation = NewObject<UEquipmentDragDropOperation>();
		OutOperation = EquipmentDragDropOperation;
		EquipmentDragDropOperation->SlotIndex = SlotIndex;
		EquipmentDragDropOperation->SlotType = SlotType;
		if (BaseItemStateRow)
		{
			if (DragWidgetClass)
			{
				UBaseSlot* DragWidget = CreateWidget<UBaseSlot>(GetWorld(), DragWidgetClass);
				if (DragWidget)
				{
					if (UTexture2D* Texture2D = Cast<UTexture2D>(BaseItemStateRow->InventoryIcon))
					{
						DragWidget->ItemImage->SetBrushFromTexture(Texture2D);
					}
					EquipmentDragDropOperation->DefaultDragVisual = DragWidget;
				}
			}
		}
	}

}


bool UEquipmentSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	if (IBaseDragDropOperationInterface* Operation = Cast<IBaseDragDropOperationInterface>(InOperation))
	{
		if (Operation->DropEquipment(SlotIndex, SlotType))
		{
			return true;
		}
	}

	return false;
}