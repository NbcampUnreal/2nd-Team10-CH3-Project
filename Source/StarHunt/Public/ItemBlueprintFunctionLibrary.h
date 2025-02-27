// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ItemSubsystem.h"
#include "ItemBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class STARHUNT_API UItemBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Item|Function")
	static UGameInstance* GetGameInstance();
	UFUNCTION(BlueprintCallable, Category = "Item|Function")

	static UGameInstanceSubsystem* GetGameInstanceSubsystem();
	UFUNCTION(BlueprintCallable, Category = "Item|Function")
	static UItemSubsystem* GetItemSubsystem();
};
