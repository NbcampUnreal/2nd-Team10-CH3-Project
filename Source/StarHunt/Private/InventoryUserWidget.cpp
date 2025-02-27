// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryUserWidget.h"
#include "ItemBlueprintFunctionLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/CanvasPanel.h"

void UInventoryUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		InventoryImages.SetNum(ItemSubsystem->GetInventoryMaxStock());
	}
	InventoryTypes = {TEXT("Gun"), TEXT("GunFixture"), TEXT("Consumable")};
	InventoryCurrentIndex = 0;

	InitInventory(ItemGridPanel);
}

void UInventoryUserWidget::SwapInventory(const int32 Index1, const int32 Index2)
{
	UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem();
	ItemSubsystem->SwapItem(InventoryTypes[InventoryCurrentIndex], Index1, Index2);
}

UTexture* UInventoryUserWidget::GetInventoryIcon(const FString& ItemType, const int32 Index)
{
	UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem();
	if (FString* ItemID = ItemSubsystem->GetInventoryItemID(ItemType, Index).Get())
	{
		if (UItemDB* ItemDB = ItemSubsystem->ItemDB)
		{
			if (FBaseItemStateRow* BaseItemStateRow = ItemDB->GetBaseItemStateRow(*ItemID))
			{
				return BaseItemStateRow->InventoryIcon;
			}
		}
	}
	return nullptr;
}

void UInventoryUserWidget::InitInventory(UGridPanel* GridPanel)
{
	int32 Index = 0;
	for (UWidget* HorizontalBoxSlot : GridPanel->GetAllChildren())
	{
		if (UItemSlot* Image = Cast<UItemSlot>(HorizontalBoxSlot))
		{
			if (!InventoryImages.IsValidIndex(Index))
			{ 
				return;
			}
			InventoryImages[Index] = Image;
			Image->SetSlotIndex(InventoryTypes[InventoryCurrentIndex], Index);
			Index++;
		}
	}
}

void UInventoryUserWidget::SetInventoryType(const int32 InventoryIndex)
{
	if (InventoryTypes.IsValidIndex(InventoryIndex))
	{

		if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
		{

			InventoryCurrentIndex = InventoryIndex;
			for (int32 Index = 0; Index < InventoryImages.Num(); Index++)
			{
				InventoryImages[Index]->SetSlotIndex(InventoryTypes[InventoryCurrentIndex], Index);
				ItemSubsystem->OnInventoryChange.Broadcast(Index);
			}
		}
	}
}

