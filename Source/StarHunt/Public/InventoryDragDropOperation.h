// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemSlot.h"
#include "BaseDragDropOperationInterface.h"
#include "InventoryDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class STARHUNT_API UInventoryDragDropOperation : public UDragDropOperation, public IBaseDragDropOperationInterface
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)
	int32 SlotIndex;
	UPROPERTY(VisibleAnywhere)
	EInventoryType SlotType;


	// IBaseDragDropOperationInterface을(를) 통해 상속됨
	bool DropInventory(int32 DropSlotIndex, EInventoryType DropSlotType) override;

	bool DropEquipment(int32 DropSlotIndex, EInventoryType DropSlotType) override;

};
