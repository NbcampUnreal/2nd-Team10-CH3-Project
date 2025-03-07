// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ItemDB.h"
#include "ShooterGameInstance.generated.h"

/**
 *
 */
UCLASS()
class STARHUNT_API UShooterGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UShooterGameInstance();

	virtual void Init() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	FString Name;
	
	// 총 점수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;
	// 총 플레이 타임
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalPlayTimeMinute;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalPlayTimeSec;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentWave;

	// 플레이어 데이터(HP, 인벤토리 등등)

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount);

	//이름 설정
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void SetName(FString Text);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UItemDB> ItemDBClass;
};
