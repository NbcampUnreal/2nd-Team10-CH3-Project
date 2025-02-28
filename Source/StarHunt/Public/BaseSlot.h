// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSubSystem.h"
#include "Components/Image.h"
#include "BaseSlot.generated.h"

/**
 * 
 */
UCLASS()
class STARHUNT_API UBaseSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetSlotIndex(EInventoryType ItemType, const int32 Index);
	virtual void UpdateSlot();
	void UpdateSlotAt(int32 Index);

	virtual void UpdateUI();


	UPROPERTY(EditAnywhere, Category = "Slot", meta = (BindWidget = "true"))
	UImage* ItemImage;
	UPROPERTY(EditAnywhere, Category = "Slot")
	UTexture2D* DefaultImageTexture;


	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;


	UPROPERTY(EditAnywhere, Category = "Slot")
	int32 SlotIndex;
	UPROPERTY(EditAnywhere, Category = "Slot")
	EInventoryType SlotType;

	UPROPERTY(EditAnywhere, Category = "Slot")
	TSubclassOf<UBaseSlot> DragWidgetClass;

	FBaseItemStateRow* BaseItemStateRow;

};
