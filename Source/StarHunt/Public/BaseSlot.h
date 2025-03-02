// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSubSystem.h"
#include "Components/Image.h"
#include "ItemDetailsWidget.h"
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
	virtual void NativeDestruct() override;

	void SetSlotIndex(EInventoryType ItemType, const int32 Index);
	virtual void UpdateSlot();
	void UpdateSlotAt(int32 Index);

	virtual void UpdateUI();


	UPROPERTY(EditAnywhere, Category = "Slot", meta = (BindWidget = "true"))
	UImage* ItemImage;
	UPROPERTY(EditAnywhere, Category = "Slot")
	UTexture2D* DefaultImageTexture;


	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Slot")
	int32 SlotIndex;
	UPROPERTY(EditAnywhere, Category = "Slot")
	EInventoryType SlotType;
	UPROPERTY(EditAnywhere, Category = "Slot")
	TSoftClassPtr<UBaseSlot> DragWidgetClass;
	UPROPERTY(EditAnywhere, Category = "Slot")
	TSubclassOf<UItemDetailsWidget> ItemDetailsWidgetClass;
	UPROPERTY(VisibleAnywhere, Category = "Slot")
	UItemDetailsWidget* ItemDetailsWidgetInstance;

	FBaseItemStateRow* BaseItemStateRow;

};
