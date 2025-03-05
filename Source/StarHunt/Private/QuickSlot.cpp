// Fill out your copyright notice in the Description page of Project Settings.

#include "QuickSlot.h"
#include "ItemBlueprintFunctionLibrary.h"
#include "Components/TextBlock.h"
#include "QuipSlotDragDropOperation.h"


void UQuickSlot::NativeConstruct()
{
	Super::NativeConstruct();

	SlotIndex = 0;
	SlotType = EInventoryType::Consumable;
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		QickSlotChangeHandler = ItemSubsystem->OnQuickSlotChange.AddUObject(this, &UQuickSlot::UpdateSlotAt);
		InventoryChangeHandler = ItemSubsystem->OnInventoryChange.AddUObject(this, &UQuickSlot::UpdateSlotAtInventory);
	}

	UpdateSlot();
}

void UQuickSlot::NativeDestruct()
{
	Super::NativeDestruct();
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		ItemSubsystem->OnQuickSlotChange.Remove(QickSlotChangeHandler);
		ItemSubsystem->OnInventoryChange.Remove(InventoryChangeHandler);
	}
}

void UQuickSlot::UpdateSlot()
{
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		InventoryItemWeakPtr = ItemSubsystem->GetQuickSlotPointInventoryIndex(SlotIndex);
		if (TSharedPtr<FInventoryItem> InventoryItem = InventoryItemWeakPtr.Pin())
		{
			if (InventoryItem->Stock > 0)
			{
				BaseItemStateRow = ItemSubsystem->GetBaseItemStateRow(InventoryItem->ItemID);
				HealingItemStateRow = ItemSubsystem->GetHealingItemStateRow(InventoryItem->ItemID);

				UpdateUI();
				return;
			}
		}
	}

	BaseItemStateRow = nullptr;
	InventoryItemWeakPtr = nullptr;
	UpdateUI();
	return;
}

void UQuickSlot::UpdateSlotAtInventory(int32 InventoryIndex)
{
	if (TSharedPtr<FInventoryItem> InventoryItem = InventoryItemWeakPtr.Pin())
	{
		if (InventoryItem->InventoryIndex == InventoryIndex)
		{
			UpdateSlot();
		}
	}
}

void UQuickSlot::UpdateUI()
{
	if (BaseItemStateRow && HealingItemStateRow)
	{
		int32 Stock = InventoryItemWeakPtr.Pin()->Stock;
		if (Stock > 1)
		{
			ItemCount->SetText(FText::AsNumber(Stock));
		}
		else
		{
			ItemCount->SetText(FText::GetEmpty());
		}
		if (ItemImage)
		{
			if (UTexture2D* Texture2D = Cast<UTexture2D>(BaseItemStateRow->InventoryIcon))
			{
				ItemImage->SetBrushFromTexture(Texture2D);
			}
			else if (DefaultImageTexture)
			{
				ItemImage->SetBrushFromTexture(DefaultImageTexture);
			}
		}
	}
	else
	{
		ItemCount->SetText(FText::GetEmpty());
		if (ItemImage && DefaultImageTexture)
		{
			ItemImage->SetBrushFromTexture(DefaultImageTexture);
		}
	}
}

void UQuickSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (OutOperation == nullptr)
	{
		UQuipSlotDragDropOperation* QuipSlotDragDropOperation = NewObject<UQuipSlotDragDropOperation>();
		OutOperation = QuipSlotDragDropOperation;
		QuipSlotDragDropOperation->SlotIndex = SlotIndex;
		QuipSlotDragDropOperation->SlotType = SlotType;
		if (BaseItemStateRow)
		{
			if (DragWidgetClass)
			{
				if (UClass* DragWidgetInstance = DragWidgetClass.LoadSynchronous())
				{
					UBaseSlot* DragWidget = CreateWidget<UBaseSlot>(GetWorld(), DragWidgetInstance);
					if (DragWidget && DragWidget->ItemImage)
					{
						DragWidget->ItemImage->SetBrushFromTexture(BaseItemStateRow->InventoryIcon);
						QuipSlotDragDropOperation->DefaultDragVisual = DragWidget;
					}
				}
			}
		}
	}

}


bool UQuickSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	if (IItemDragDropOperationInterface* Operation = Cast<IItemDragDropOperationInterface>(InOperation))
	{
		if (Operation->DropQuickSlot(SlotIndex, SlotType))
		{
			UpdateSlot();
			return true;
		}
	}

	return false;
}

void UQuickSlot::NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	Super::NativeOnMouseEnter(MyGeometry, MouseEvent);

	if (ItemDetailsWidgetInstance)
	{
		if (HealingItemStateRow)
		{
			ItemDetailsWidgetInstance->SetItem(BaseItemStateRow, HealingItemStateRow);
		}
	}
}