// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSlot.h"
#include "ItemSubsystem.h"
#include "QuickSlot.generated.h"

/**
 * 
 */
UCLASS()
class STARHUNT_API UQuickSlot : public UBaseSlot
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void UpdateSlot() override;
	void UpdateSlotAtInventory(int32 InventoryIndex) ;
	virtual void UpdateUI() override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	virtual void NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	TWeakPtr<FInventoryItem> InventoryItemWeakPtr;
	FHealingItemStateRow* HealingItemStateRow;

	UPROPERTY(EditAnywhere, Category = "Slot", meta = (BindWidget = "true"))
	UTextBlock* ItemCount;

	FDelegateHandle QickSlotChangeHandler;
	FDelegateHandle InventoryChangeHandler;

};
