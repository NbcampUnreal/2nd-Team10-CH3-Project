// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItemSlot.h"
#include "DropItemDragDropOperation.h"
#include "ItemBlueprintFunctionLibrary.h"
#include "DropItemActor.h"

void UDropItemSlot::UpdateSlot()
{
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		if (ItemID)
		{
			if (ItemID->IsValid())
			{
				BaseItemStateRow = ItemSubsystem->GetBaseItemStateRow(**ItemID);
				if (BaseItemStateRow)
				{
					GunItemStateRow = ItemSubsystem->GetGunItemStateRow(**ItemID);
					if (GunItemStateRow)
					{
						GunFixtureItemStateRow = nullptr;
						HealingItemStateRow = nullptr;
					}
					GunFixtureItemStateRow = ItemSubsystem->GetGunFixtureItemStateRow(**ItemID);
					if (GunFixtureItemStateRow)
					{
						GunItemStateRow = nullptr;
						HealingItemStateRow = nullptr;
					}
					HealingItemStateRow = ItemSubsystem->GetHealingItemStateRow(**ItemID);
					if (HealingItemStateRow)
					{
						GunItemStateRow = nullptr;
						GunFixtureItemStateRow = nullptr;
					}
					UpdateUI();
					return;
				}
			}

		}
	}

	GunItemStateRow = nullptr;
	GunFixtureItemStateRow = nullptr;
	HealingItemStateRow = nullptr;
	BaseItemStateRow = nullptr;
	UpdateUI();
}

void UDropItemSlot::UpdateUI()
{
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		if (BaseItemStateRow)
		{
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

void UDropItemSlot::Init(TSharedPtr<FString>* ItemId)
{
	ItemID = ItemId;
	UpdateSlot();
}

void UDropItemSlot::Init(ADropItemActor* ItemActor, TSharedPtr<FString>* ItemId)
{
	ItemID = ItemId;
	DropItemActor = ItemActor;
	UpdateSlot();
}

void UDropItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (OutOperation == nullptr)
	{
		UDropItemDragDropOperation* DropItemDragDropOperation = NewObject<UDropItemDragDropOperation>();
		OutOperation = DropItemDragDropOperation;
		DropItemDragDropOperation->DropItem = ItemID;
		DropItemDragDropOperation->DropItemActor = DropItemActor;
		if (DropItemActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("ADFGBA"));
		}
		DropItemDragDropOperation->DropItemSlot = this;

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
						DropItemDragDropOperation->DefaultDragVisual = DragWidget;
					}
				}
			}
		}
	}

}

void UDropItemSlot::NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
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