// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawnComponent.h"
#include "SpawnItemRow.h"
#include "DropItemActor.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UItemSpawnComponent::UItemSpawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...

}

void UItemSpawnComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

}

bool UItemSpawnComponent::bIsSpawnedItem(float Rate)
{
	if (Rate < UKismetMathLibrary::RandomFloat())
	{
		return true;
	}
	return false;
}

AActor* UItemSpawnComponent::SpawnedItem()
{
	TArray<FSpawnItemRow*> SpawnItemRows;
	SpawnRateTable->GetAllRows(FString("SpawnItemRows"), SpawnItemRows);
	TArray<FString> DropItemIDs;

	for (FSpawnItemRow* SpawnItemRow : SpawnItemRows)
	{
		if (bIsSpawnedItem(SpawnItemRow->SpawnRate))
		{
			DropItemIDs.Add(SpawnItemRow->ItemID);
		}
	}

	if (GetWorld() && GetOwner())
	{
		if (ADropItemActor* DropItemActor = GetWorld()->SpawnActor<ADropItemActor>(DropItemActorClass, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation()))
		{
			DropItemActor->SetItemIDs(DropItemIDs);
			return DropItemActor;
		}
	}
	return nullptr;
}



