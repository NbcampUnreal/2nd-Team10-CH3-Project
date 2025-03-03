// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveableWidget.h"
#include "MoveWindowDragDropOperation.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"




void UMoveableWidget::NativeConstruct()
{
	WidgetPos = FVector2D::ZeroVector;
}

bool UMoveableWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (InOperation)
	{
		if (UMoveWindowDragDropOperation* MoveWindowDragDropOperation = Cast<UMoveWindowDragDropOperation>(InOperation))
		{
			if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(MoveCanvasPanel->Slot))
			{
				FVector2D Move = InDragDropEvent.GetScreenSpacePosition();
				CanvasSlot->SetPosition(CanvasSlot->GetPosition() + Move - MoveWindowDragDropOperation->StartScreenLocation);

				MoveWindowDragDropOperation->StartScreenLocation = Move;
			}
			
		}
	}
	return Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
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

