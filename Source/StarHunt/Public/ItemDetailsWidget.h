// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemStateRow.h"
#include "ItemDetailsWidget.generated.h"

class UTextBlock;
class UGridPanel;

/**
 * 
 */
UCLASS()
class STARHUNT_API UItemDetailsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetItemID(const FString& Name);
	void SetItemID(const FText& Name);
	void SetItem(FBaseItemStateRow* BaseItemStateRow, FHealingItemStateRow* HealingItemStateRow);
	void SetItem(FBaseItemStateRow* BaseItemStateRow, FGunItemStateRow* GunItemStateRow);
	void SetItem(FBaseItemStateRow* BaseItemStateRow, FGunFixtureItemStateRow* GunFixtureItemStateRow);

	void AddText(const FText& ImportText);

	void RemoveAll();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UGridPanel* DetailsGridPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWidgetTransform TextTransform;


};
