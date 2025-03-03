// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/GridPanel.h"
#include "Components/Border.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/Button.h"

#include "ItemSubsystem.h"
#include "ItemSlot.h"

#include "InventoryUserWidget.generated.h"

class UCanvasPanel;
/**
 * 
 */
UCLASS()
class STARHUNT_API UInventoryUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void InitInventory(UGridPanel* GridPanel);

	UFUNCTION(BlueprintCallable)
	void SetInventoryType(EInventoryType Type);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UItemSlot*> InventoryImages;
	UPROPERTY(meta = (BindWidget))
	UGridPanel* ItemGridPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInventoryType InventoryType;


	virtual void NativeConstruct() override;
};
