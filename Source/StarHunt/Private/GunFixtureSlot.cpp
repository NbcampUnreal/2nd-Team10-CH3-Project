// Fill out your copyright notice in the Description page of Project Settings.


#include "GunFixtureSlot.h"
#include "ItemBlueprintFunctionLibrary.h"
#include "GunFixtureDragDropOperation.h"

void UGunFixtureSlot::NativeConstruct()
{
	Super::NativeConstruct();
	SlotType = EInventoryType::GunFixture;

	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		GunFixtureChangeHandler = ItemSubsystem->OnGunFixtureChange.AddUObject(this, &UGunFixtureSlot::UpdateSlotAt);
	}

	UpdateSlot();
}

void UGunFixtureSlot::NativeDestruct()
{
	Super::NativeDestruct();
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		ItemSubsystem->OnEquipmentChange.Remove(GunFixtureChangeHandler);
	}
}

void UGunFixtureSlot::UpdateSlot()
{
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		if (TSharedPtr<FString> ItemID = ItemSubsystem->GetEquipmentGunFixtureItemID(SlotIndex, GunFixtureType))
		{

			if (ItemID.IsValid())
			{
				BaseItemStateRow = ItemSubsystem->GetBaseItemStateRow(*ItemID);
				GunFixtureItemStateRow = ItemSubsystem->GetGunFixtureItemStateRow(*ItemID);
			}

			UpdateUI();
			return;
		}
	}

	BaseItemStateRow = nullptr;
	GunFixtureItemStateRow = nullptr;
	UpdateUI();
}

void UGunFixtureSlot::UpdateUI()
{
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		if (ItemImage)
		{
			if (ItemSubsystem->ValidGunFixtureEquipment(SlotIndex, GunFixtureType))
			{

				if (BaseItemStateRow)
				{

					if (UTexture2D* Texture2D = Cast<UTexture2D>(BaseItemStateRow->InventoryIcon))
					{
						ItemImage->SetVisibility(ESlateVisibility::Visible);
						ItemImage->SetBrushFromTexture(Texture2D);
						return;

					}

				}
				else
				{
					ItemImage->SetVisibility(ESlateVisibility::Visible);
					ItemImage->SetBrushFromTexture(DefaultImageTexture);
					return;
				}
			}
			else
			{
				ItemImage->SetVisibility(ESlateVisibility::Hidden);
			}

		}
		
	}
}

void UGunFixtureSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (OutOperation == nullptr)
	{
		UGunFixtureDragDropOperation* GunFixtureDragDropOperation = NewObject<UGunFixtureDragDropOperation>();
		OutOperation = GunFixtureDragDropOperation;
		GunFixtureDragDropOperation->SlotIndex = SlotIndex;
		GunFixtureDragDropOperation->SlotType = SlotType;
		GunFixtureDragDropOperation->GunFixtureType = GunFixtureType;
		if (BaseItemStateRow)
		{
			if (DragWidgetClass)
			{
				if (UClass* DragWidgetInstance = DragWidgetClass.LoadSynchronous())
				{
					UBaseSlot* DragWidget = CreateWidget<UBaseSlot>(GetWorld(), DragWidgetInstance);
					if (DragWidget)
					{
						if (UTexture2D* Texture2D = Cast<UTexture2D>(BaseItemStateRow->InventoryIcon))
						{
							DragWidget->ItemImage->SetBrushFromTexture(Texture2D);
						}
						GunFixtureDragDropOperation->DefaultDragVisual = DragWidget;
					}
				}
			}
		}
	}

}


bool UGunFixtureSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}

void UGunFixtureSlot::NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	Super::NativeOnMouseEnter(MyGeometry, MouseEvent);

	if (ItemDetailsWidgetInstance && GunFixtureItemStateRow)
	{
		ItemDetailsWidgetInstance->SetItem(BaseItemStateRow, GunFixtureItemStateRow);
	}
}

void UGunFixtureSlot::UpdateSlotAt(int32 EquipmentIndex, EGunFixtureType Type)
{
	if (SlotIndex == EquipmentIndex && GunFixtureType == Type)
	{
		UpdateSlot();
	}
}
