// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemSpawnComponent.generated.h"

class ADropItemActor;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STARHUNT_API UItemSpawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemSpawnComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Spawn")
	UDataTable* SpawnRateTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Spawn")
	TSubclassOf<ADropItemActor> DropItemActorClass;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	bool bIsSpawnedItem(float Rate);

	AActor* SpawnedItem();
};
