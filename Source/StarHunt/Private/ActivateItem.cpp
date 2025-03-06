// Fill out your copyright notice in the Description page of Project Settings.


#include "ActivateItem.h"
#include "ItemBlueprintFunctionLibrary.h"

UActivateItem::UActivateItem()
{
}

//일단 하나만
IActivateItemInterface* UActivateItem::GetActivateActionClass()
{
	return NewObject<UHealingItemActivate>(UHealingItemActivate::StaticClass());
}
