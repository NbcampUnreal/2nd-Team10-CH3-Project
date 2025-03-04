// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemEnumClass.h"
#include "ItemDragDropOperationInterface.h"
#include "DropItemSlot.h"
#include "DropItemDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class STARHUNT_API UDropItemDragDropOperation : public UDragDropOperation, public IItemDragDropOperationInterface
{
	GENERATED_BODY()

public:
	TSharedPtr<FString>* DropItem;
	UDropItemSlot* DropItemSlot;
	ADropItemActor* DropItemActor;

	// IItemDragDropOperationInterface을(를) 통해 상속됨
	bool DropInventory(int32 DropSlotIndex, EInventoryType DropSlotType) override;

	bool DropEquipment(int32 DropSlotIndex, EInventoryType DropSlotType) override;

};
