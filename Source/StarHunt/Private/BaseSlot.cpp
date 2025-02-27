// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSlot.h"
#include "ItemBlueprintFunctionLibrary.h"
#include "InventoryDragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UBaseSlot::NativeConstruct()
{
	Super::NativeConstruct();
	if (ItemImage)
	{
		ItemImage->SetBrushFromTexture(DefaultImageTexture);
	}
}

void UBaseSlot::SetSlotIndex(const FString& ItemType, const int32 Index)
{
	SlotType = ItemType;
	SlotIndex = Index;

	UpdateSlot();

}

void UBaseSlot::UpdateSlot()
{
}

void UBaseSlot::UpdateSlotAt(int32 Index)
{
	if (Index == SlotIndex)
	{
		UpdateSlot();
	}
}

void UBaseSlot::UpdateUI()
{
}

FReply UBaseSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		if (BaseItemStateRow)
		{
			Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		}
	}
	return Reply.NativeReply;
}
