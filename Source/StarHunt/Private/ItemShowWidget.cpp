// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemShowWidget.h"


void UItemShowWidget::ShowInventory()
{
    UserInventoryWidget->SetVisibility(ESlateVisibility::Visible);
}

void UItemShowWidget::CloseInventory()
{
    UserInventoryWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UItemShowWidget::ShowEquipment()
{
    EquipmentWidget->SetVisibility(ESlateVisibility::Visible);
}

void UItemShowWidget::CloseEquipment()
{
    EquipmentWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UItemShowWidget::ShowDropItems(TArray<ADropItemActor*>* DropItemActors)
{
    if (DropItemActors && DropItemActors->Num() > 0)
    {
        DropInventoryWidget->InitInventory(DropItemActors);
        DropInventoryWidget->SetVisibility(ESlateVisibility::Visible);
    }
}
void UItemShowWidget::CloseDropItems()
{
    DropInventoryWidget->SetVisibility(ESlateVisibility::Hidden);
}