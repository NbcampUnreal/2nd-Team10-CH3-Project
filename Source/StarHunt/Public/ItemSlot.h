// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSubsystem.h"
#include "BaseSlot.h"
#include "Components/Image.h"
#include "ItemSlot.generated.h"

/**
 * 
 */
UCLASS()
class STARHUNT_API UItemSlot : public UBaseSlot
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void UpdateSlot() override;
	virtual void UpdateUI() override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;


	FDelegateHandle InventoryChangeHandler;
};
