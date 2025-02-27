// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "BaseDragDropOperationInterface.h"
#include "EquipmentDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class STARHUNT_API UEquipmentDragDropOperation : public UDragDropOperation, public IBaseDragDropOperationInterface
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)
	int32 SlotIndex;
	UPROPERTY(VisibleAnywhere)
	FString SlotType;

	

	// IBaseDragDropOperationInterface을(를) 통해 상속됨
	bool DropInventory(int32 DropSlotIndex, const FString& DropSlotType) override;

	bool DropEquipment(int32 DropSlotIndex, const FString& DropSlotType) override;

};
