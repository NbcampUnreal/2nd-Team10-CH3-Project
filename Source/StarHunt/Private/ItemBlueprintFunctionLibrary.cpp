// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBlueprintFunctionLibrary.h"
#include "ItemSubsystem.h"
#include "Engine/GameInstance.h"

UGameInstance* UItemBlueprintFunctionLibrary::GetGameInstance()
{
    if (GEngine && GEngine->GameViewport)
    {
        return GEngine->GameViewport->GetGameInstance();
    }
    return nullptr;
}

UGameInstanceSubsystem* UItemBlueprintFunctionLibrary::GetGameInstanceSubsystem()
{
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        return GameInstance->GetSubsystem<UItemSubsystem>();
    }
    return nullptr;
}

UItemSubsystem* UItemBlueprintFunctionLibrary::GetItemSubsystem()
{
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        return GameInstance->GetSubsystem<UItemSubsystem>();
    }
    return nullptr;
}
