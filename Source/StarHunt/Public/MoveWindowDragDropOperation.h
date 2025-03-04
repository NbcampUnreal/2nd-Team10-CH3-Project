// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "MoveWindowDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class STARHUNT_API UMoveWindowDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	FVector2D StartScreenLocation;
};
