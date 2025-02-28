// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSlot.h"
#include "GunFixtureSlot.generated.h"

/**
 * 
 */
UCLASS()
class STARHUNT_API UGunFixtureSlot : public UBaseSlot
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void UpdateSlot() override;
	virtual void UpdateUI() override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

	void UpdateSlotAt(int32 EquipmentIndex, EGunFixtureType Type);

	EGunFixtureType GunFixtureType;
	FGunFixtureItemStateRow* GunFixtureItemStateRow;
	FDelegateHandle GunFixtureChangeHandler;
};
