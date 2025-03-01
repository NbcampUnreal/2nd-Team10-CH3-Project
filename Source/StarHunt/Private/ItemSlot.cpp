// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlot.h"
#include "ItemBlueprintFunctionLibrary.h"
#include "InventoryDragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	SlotIndex = 0;
	SlotType = EInventoryType::Gun;
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		InventoryChangeHandler = ItemSubsystem->OnInventoryChange.AddUObject(this, &UItemSlot::UpdateSlotAt);
	}
	UpdateSlot();
}

void UItemSlot::NativeDestruct()
{
	Super::NativeDestruct();
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		ItemSubsystem->OnInventoryChange.Remove(InventoryChangeHandler);
	}
}

void UItemSlot::UpdateSlot()
{
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		if (TSharedPtr<FString> ItemID = ItemSubsystem->GetInventoryItemID(SlotType, SlotIndex))
		{
			BaseItemStateRow = ItemSubsystem->GetBaseItemStateRow(*ItemID);

			UpdateUI();
			return;
		}
	}

	BaseItemStateRow = nullptr;
	UpdateUI();
}

void UItemSlot::UpdateUI()
{
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		if (TSharedPtr<FString> ItemID = ItemSubsystem->GetInventoryItemID(SlotType, SlotIndex))
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

		if (ItemImage && DefaultImageTexture)
		{
			ItemImage->SetBrushFromTexture(DefaultImageTexture);
		}
	}
}

void UItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (OutOperation == nullptr)
	{
		UInventoryDragDropOperation* InventoryDragDropOperation = NewObject<UInventoryDragDropOperation>();
		OutOperation = InventoryDragDropOperation;
		InventoryDragDropOperation->SlotIndex = SlotIndex;
		InventoryDragDropOperation->SlotType = SlotType;
		if (BaseItemStateRow)
		{
			if (DragWidgetClass)
			{
				UBaseSlot* DragWidget = CreateWidget<UBaseSlot>(GetWorld(), DragWidgetClass);
				if (DragWidget)
				{
					DragWidget->ItemImage->SetBrush(ItemImage->GetBrush());
					InventoryDragDropOperation->DefaultDragVisual = DragWidget;
				}
			}
		}
	}

}


bool UItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	if (IItemDragDropOperationInterface* Operation = Cast<IItemDragDropOperationInterface>(InOperation))
	{
		if (Operation->DropInventory(SlotIndex, SlotType))
		{
			return true;
		}
	}

	return false;
}
