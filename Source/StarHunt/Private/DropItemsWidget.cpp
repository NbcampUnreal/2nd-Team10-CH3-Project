// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItemsWidget.h"
#include "DropItemActor.h"
#include "Components/GridSlot.h"


void UDropItemsWidget::NativeOnInitialized()
{
	MaxRow = 4;
}

void UDropItemsWidget::NativeDestruct()
{
	if (ItemGridPanel)
	{
		for (UWidget* Widget : ItemGridPanel->GetAllChildren())
		{
			Widget->RemoveFromParent();
			Widget = nullptr;
		}
	}
}

void UDropItemsWidget::InitInventory(TArray<TSharedPtr<FString>>* DropItems)
{
	if (DropItems)
	{
		if (UClass* Class = DropItemSlotClass.LoadSynchronous())
		{
			for (int32 Index = 0; Index < DropItems->Num(); Index++)
			{
				TSharedPtr<FString> ItemIdPtr = (*DropItems)[Index];
				if (ItemIdPtr.IsValid())
				{
					if (UDropItemSlot* DropItemSlot = CreateWidget<UDropItemSlot>(GetWorld(), Class))
					{
						UGridSlot* GridSlot = ItemGridPanel->AddChildToGrid(DropItemSlot);
						DropItemSlot->Init(ItemIdPtr);
						GridSlot->SetRow(Index / MaxRow);
						GridSlot->SetColumn(Index % MaxRow);
					}
				}
			}
		}
	}
}
void UDropItemsWidget::InitInventory(TArray<ADropItemActor*>* DropItemActors)
{
	if (DropItemActors)
	{
		if (UClass* Class = DropItemSlotClass.LoadSynchronous())
		{
			int32 Index = 0;
			ItemGridPanel->ClearChildren();
			for (ADropItemActor* DropItemActor : *DropItemActors)
			{
				for (TSharedPtr<FString>& ItemIDPtr : DropItemActor->ItemIDs)
				{
					if (ItemIDPtr.IsValid())
					{
						if (UDropItemSlot* DropItemSlot = CreateWidget<UDropItemSlot>(GetWorld(), Class))
						{
							UGridSlot* GridSlot = ItemGridPanel->AddChildToGrid(DropItemSlot);
							DropItemSlot->Init(DropItemActor, ItemIDPtr);
							DropItemSlot->SlotIndex = Index;
							GridSlot->SetRow(Index / MaxRow);
							GridSlot->SetColumn(Index % MaxRow);
							Index++;
						}
					}
				}
			}
		}
	}
}