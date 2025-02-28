// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemEnumClass.h"
#include "ItemDragDropOperationInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemDragDropOperationInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STARHUNT_API IItemDragDropOperationInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool DropInventory(int32 DropSlotIndex, EInventoryType DropSlotType) = 0;
	virtual bool DropEquipment(int32 DropSlotIndex, EInventoryType DropSlotType) = 0;
};
