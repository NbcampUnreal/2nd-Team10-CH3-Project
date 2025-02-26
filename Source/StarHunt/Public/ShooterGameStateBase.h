// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ShooterGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class STARHUNT_API AShooterGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
	AShooterGameStateBase();
 
public:
	// 현재 남은 적의 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 RemainEnemeies;

	// 현재 레벨
	// 
};
