// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSlot.h"
#include "EquipmentSlot.generated.h"

class UGunFixtureSlot;
/**
 * 
 */
UCLASS()
class STARHUNT_API UEquipmentSlot : public UBaseSlot
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void UpdateSlot() override;
	virtual void UpdateUI() override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	virtual void NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	UPROPERTY(EditAnywhere, Category = "Slot", meta = (BindWidget = "true"))
	UGunFixtureSlot* Muffle;
	UPROPERTY(EditAnywhere, Category = "Slot", meta = (BindWidget = "true"))
	UGunFixtureSlot* Magazine;

	FGunItemStateRow* GunItemStateRow;

	FDelegateHandle EquipmentChangeHandler;
};
