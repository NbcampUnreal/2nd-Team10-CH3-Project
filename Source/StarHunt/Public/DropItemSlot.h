// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSlot.h"
#include "DropItemSlot.generated.h"

class ADropItemActor;
/**
 * 
 */
UCLASS()
class STARHUNT_API UDropItemSlot : public UBaseSlot
{
	GENERATED_BODY()
	
public:
	void Init(TSharedPtr<FString>* ItemId);
	void Init(ADropItemActor* ItemActor, TSharedPtr<FString>* ItemId);

	virtual void UpdateSlot() override;
	virtual void UpdateUI() override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	virtual void NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	TSharedPtr<FString>* ItemID;
	ADropItemActor* DropItemActor;

	FGunItemStateRow* GunItemStateRow;
	FGunFixtureItemStateRow* GunFixtureItemStateRow;
	FHealingItemStateRow* HealingItemStateRow;
};
