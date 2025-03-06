// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "DragItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class STARHUNT_API UDragItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Slot", meta = (BindWidget = "true"))
	UImage* ItemImage;
};
