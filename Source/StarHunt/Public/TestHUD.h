// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TestHUD.generated.h"

/**
 * 
 */
UCLASS()
class STARHUNT_API ATestHUD : public AHUD
{
	GENERATED_BODY()
	
protected:

public:
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	UTexture2D* CrosshairTexture;
};
