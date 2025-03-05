// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveableWidget.h"
#include "MoveWindowDragDropOperation.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"




void UMoveableWidget::NativeConstruct()
{
	WidgetPos = FVector2D::ZeroVector;
}

bool UMoveableWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
	if (InOperation)
	{
		if (UMoveWindowDragDropOperation* MoveWindowDragDropOperation = Cast<UMoveWindowDragDropOperation>(InOperation))
		{
			if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(MoveCanvasPanel->Slot))
			{
				FVector2D Move = InDragDropEvent.GetScreenSpacePosition();

				FVector2D LocalPos = InGeometry.AbsoluteToLocal(Move);

				CanvasSlot->SetPosition(LocalPos);
				//CanvasSlot->SetPosition(InGeometry.GetLocalSize() / 2 - InDragDropEvent.GetScreenSpacePosition());

				MoveWindowDragDropOperation->StartScreenLocation = Move;
				return true;
			}
		}
	}
	return false;
}

bool UMoveableWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (InOperation)
	{

		if (UMoveWindowDragDropOperation* MoveWindowDragDropOperation = Cast<UMoveWindowDragDropOperation>(InOperation))
		{
			return false;
		}
	}
	return false;
}

