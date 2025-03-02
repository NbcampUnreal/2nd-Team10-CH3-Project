// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDetailsWidget.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "Components/GridPanel.h"


void UItemDetailsWidget::SetItemID(const FString& Name)
{
	ItemName->SetText(FText::FromString(Name));
}

void UItemDetailsWidget::SetItemID(const FText& Name)
{
	ItemName->SetText(Name);
}

void UItemDetailsWidget::SetItem(FBaseItemStateRow* BaseItemStateRow, FHealingItemStateRow* HealingItemStateRow)
{
	if (BaseItemStateRow && HealingItemStateRow)
	{
		ItemName->SetText(BaseItemStateRow->Name);
		AddText(FText::FromString(FString::Printf(TEXT("Heal : %0.0f"),HealingItemStateRow->HealingAmount)));
	}
}

void UItemDetailsWidget::SetItem(FBaseItemStateRow* BaseItemStateRow, FGunItemStateRow* GunItemStateRow)
{

	if (BaseItemStateRow && GunItemStateRow)
	{
		if (ItemName)
		{
			ItemName->SetText(BaseItemStateRow->Name);
		}
		AddText(FText::FromString(FString::Printf(TEXT("Damage : %0.0f"), GunItemStateRow->DamageAmount)));
		AddText(FText::FromString(FString::Printf(TEXT("FireRate : %0.0f"), GunItemStateRow->FireRate)));
		AddText(FText::FromString(FString::Printf(TEXT("MaxAmmo : %0.0f"), GunItemStateRow->MaxAmmo)));
	}
}

void UItemDetailsWidget::SetItem(FBaseItemStateRow* BaseItemStateRow, FGunFixtureItemStateRow* GunFixtureItemStateRow)
{
	if (BaseItemStateRow && GunFixtureItemStateRow)
	{
		ItemName->SetText(BaseItemStateRow->Name);
		AddText(FText::FromString(FString::Printf(TEXT("FixtureType : "))));
		AddText(FText::FromString(FString::Printf(TEXT("Damage : %0.0f"), GunFixtureItemStateRow->DamageAmount)));
		AddText(FText::FromString(FString::Printf(TEXT("Ammo : %d"), GunFixtureItemStateRow->MaxAmmoAmount)));
	}
}


void UItemDetailsWidget::AddText(const FText& ImportText)
{
	if (DetailsGridPanel)
	{
		UTextBlock* TextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ItemDetailsTextBox"));

		TextBlock->SetText(ImportText);
		DetailsGridPanel->AddChildToGrid(TextBlock, DetailsGridPanel->GetChildrenCount(), 0);
		TextBlock->SetRenderTransform(TextTransform);
	}
}

void UItemDetailsWidget::RemoveAll()
{
	if (DetailsGridPanel)
	{
		for (UWidget* Widget : DetailsGridPanel->GetAllChildren())
		{
			if (Widget)
			{
				Widget->RemoveFromParent();
			}
		}
	}
}
