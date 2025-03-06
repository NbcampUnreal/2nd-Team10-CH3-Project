// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSlot.h"
#include "EquipmentDragDropOperation.h"
#include "GunFixtureSlot.h"
#include "ItemBlueprintFunctionLibrary.h"

void UEquipmentSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (Muffle)
	{
		Muffle->SlotIndex = SlotIndex;
		Muffle->GunFixtureType = EGunFixtureType::Muffle;
	}
	if (Magazine)
	{
		Magazine->SlotIndex = SlotIndex;
		Magazine->GunFixtureType = EGunFixtureType::Magazine;
	}
	SlotType = EInventoryType::Gun;
	
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		EquipmentChangeHandler = ItemSubsystem->OnEquipmentChange.AddUObject(this, &UEquipmentSlot::UpdateSlotAt);
	}

	UpdateSlot();
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
			GunItemStateRow = ItemSubsystem->GetGunItemStateRow(*ItemID);
			UpdateUI();
			return;
		}
	}

	BaseItemStateRow = nullptr;
	GunItemStateRow = nullptr;
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
				if (UClass* DragWidgetInstance = DragWidgetClass.LoadSynchronous())
				{
					UDragItemWidget* DragWidget = CreateWidget<UDragItemWidget>(GetWorld(), DragWidgetInstance);
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

}


bool UEquipmentSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	if (IItemDragDropOperationInterface* Operation = Cast<IItemDragDropOperationInterface>(InOperation))
	{
		if (Operation->DropEquipment(SlotIndex, SlotType))
		{
			return true;
		}
	}

	return false;
}

void UEquipmentSlot::NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	Super::NativeOnMouseEnter(MyGeometry, MouseEvent);

	if (ItemDetailsWidgetInstance && GunItemStateRow)
	{
		ItemDetailsWidgetInstance->SetItem(BaseItemStateRow, GunItemStateRow);
	}
}
