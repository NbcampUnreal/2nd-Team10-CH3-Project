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
	InventoryType = EInventoryType::Gun;

	InitInventory(ItemGridPanel);
}

void UInventoryUserWidget::SwapInventory(const int32 Index1, const int32 Index2)
{
	UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem();
	ItemSubsystem->SwapItem(InventoryType, Index1, Index2);
}

UTexture* UInventoryUserWidget::GetInventoryIcon(EInventoryType ItemType, const int32 Index)
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
			Image->SetSlotIndex(InventoryType, Index);
			Index++;
		}
	}
}

void UInventoryUserWidget::SetInventoryType(EInventoryType Type)
{

	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		for (int32 Index = 0; Index < InventoryImages.Num(); Index++)
		{
			InventoryType = Type;
			InventoryImages[Index]->SetSlotIndex(InventoryType, Index);
			ItemSubsystem->OnInventoryChange.Broadcast(Index);
		}
	}
}

