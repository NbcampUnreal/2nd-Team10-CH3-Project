// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSlot.h"
#include "ItemBlueprintFunctionLibrary.h"
#include "InventoryDragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UBaseSlot::NativeConstruct()
{
	Super::NativeConstruct();
	if (ItemImage)
	{
		ItemImage->SetBrushFromTexture(DefaultImageTexture);
	}
}

void UBaseSlot::NativeDestruct()
{
	Super::NativeDestruct();
	if (ItemDetailsWidgetInstance)
	{
		ItemDetailsWidgetInstance->RemoveFromParent();
		ItemDetailsWidgetInstance = nullptr;
	}
}

void UBaseSlot::SetSlotIndex(EInventoryType ItemType, const int32 Index)
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

void UBaseSlot::NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	Super::NativeOnMouseEnter(MyGeometry, MouseEvent);

	if (!ItemDetailsWidgetInstance)
	{
		if (ItemDetailsWidgetClass)
		{
			// SoftPtr로 시도했으나 이상하게도 ItemDetailsWidgetClass를 받아오지못함..?
			//if (UClass* Class = ItemDetailsWidgetClass.LoadSynchronous())
			//{
			//	ItemDetailsWidgetInstance = CreateWidget<UItemDetailsWidget>(this, Class);
			//}
			ItemDetailsWidgetInstance = CreateWidget<UItemDetailsWidget>(this, ItemDetailsWidgetClass);

			if (ItemDetailsWidgetInstance)
			{
				ItemDetailsWidgetInstance->AddToViewport();
			}

		}
		
	}
}

void UBaseSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (ItemDetailsWidgetInstance)
	{
		ItemDetailsWidgetInstance->RemoveFromParent();
		ItemDetailsWidgetInstance = nullptr;
	}
}

void UBaseSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (ItemDetailsWidgetInstance && GetWorld())
	{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			FVector2D MousePosition;
			PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);

			ItemDetailsWidgetInstance->SetPositionInViewport(MousePosition, false);
		}
	}

}
