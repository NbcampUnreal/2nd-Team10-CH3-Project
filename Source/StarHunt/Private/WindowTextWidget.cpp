// Fill out your copyright notice in the Description page of Project Settings.


#include "WindowTextWidget.h"
#include "MoveWindowDragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

FReply UWindowTextWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	}
	return Reply.NativeReply;
}

void UWindowTextWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (OutOperation == nullptr)
	{
		UMoveWindowDragDropOperation* MoveWindowDragDropOperation = NewObject<UMoveWindowDragDropOperation>();
		MoveWindowDragDropOperation->StartScreenLocation = GetCachedGeometry().GetAbsolutePosition();
		OutOperation = MoveWindowDragDropOperation;
	}
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UWindowTextWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
}
