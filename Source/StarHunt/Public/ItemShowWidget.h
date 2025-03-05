// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DropItemActor.h"
#include "DropItemsWidget.h"
#include "ItemShowWidget.generated.h"

/**
 * 
 */
UCLASS()
class STARHUNT_API UItemShowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ShowInventory();
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void CloseInventory();
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ShowEquipment();
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void CloseEquipment();

	void ShowDropItems(TArray<ADropItemActor*>* DropItemActors);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void CloseDropItems();

	UPROPERTY(meta = (BindWidget))
	UUserWidget* EquipmentWidget;
	UPROPERTY(meta = (BindWidget))
	UUserWidget* UserInventoryWidget;
	UPROPERTY(meta = (BindWidget))
	UDropItemsWidget* DropInventoryWidget;
};
