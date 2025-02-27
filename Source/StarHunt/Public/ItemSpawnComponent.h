// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemSpawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STARHUNT_API UItemSpawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemSpawnComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Spawn")
	UDataTable* SpawnRateTable;
public:	

	AActor* DropItem();
	AActor* DropItem(FVector& Location);
	AActor* DropItem(FVector& Location, FRotator& Rotator);
};
