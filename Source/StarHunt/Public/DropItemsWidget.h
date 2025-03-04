// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DropItemSlot.h"
#include "Components/GridPanel.h"
#include "DropItemsWidget.generated.h"

class ADropItemActor;
/**
 * 
 */
UCLASS()
class STARHUNT_API UDropItemsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	void InitInventory(TArray<TSharedPtr<FString>>* DropItems);
	void InitInventory(TArray<ADropItemActor*>* DropItems);

	UPROPERTY(meta = (BindWidget))
	UGridPanel* ItemGridPanel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UDropItemSlot> DropItemSlotClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxRow;
};
