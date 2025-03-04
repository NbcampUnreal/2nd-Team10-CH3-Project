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
			GunItemStateRow = ItemSubsystem->GetGunItemStateRow(*ItemID);
			if (GunItemStateRow)
			{
				GunFixtureItemStateRow = nullptr;
				HealingItemStateRow = nullptr;
			}
			GunFixtureItemStateRow = ItemSubsystem->GetGunFixtureItemStateRow(*ItemID);
			if (GunFixtureItemStateRow)
			{
				GunItemStateRow = nullptr;
				HealingItemStateRow = nullptr;
			}
			HealingItemStateRow = ItemSubsystem->GetHealingItemStateRow(*ItemID);
			if (HealingItemStateRow)
			{
				GunItemStateRow = nullptr;
				GunFixtureItemStateRow = nullptr;
			}

			UpdateUI();
			return;
		}
	}

	GunItemStateRow = nullptr;
	GunFixtureItemStateRow = nullptr;
	HealingItemStateRow = nullptr;
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
				if (UClass* DragWidgetInstance = DragWidgetClass.LoadSynchronous())
				{
					UBaseSlot* DragWidget = CreateWidget<UBaseSlot>(GetWorld(), DragWidgetInstance);
					if (DragWidget)
					{
						DragWidget->ItemImage->SetBrush(ItemImage->GetBrush());
						InventoryDragDropOperation->DefaultDragVisual = DragWidget;
					}
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

void UItemSlot::NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	Super::NativeOnMouseEnter(MyGeometry, MouseEvent);

	if (ItemDetailsWidgetInstance)
	{

		if (GunItemStateRow)
		{
			ItemDetailsWidgetInstance->SetItem(BaseItemStateRow, GunItemStateRow);
		}
		else if (GunFixtureItemStateRow)
		{
			ItemDetailsWidgetInstance->SetItem(BaseItemStateRow, GunFixtureItemStateRow);
		}
		else if (HealingItemStateRow)
		{
			ItemDetailsWidgetInstance->SetItem(BaseItemStateRow, HealingItemStateRow);
		}
	}
}