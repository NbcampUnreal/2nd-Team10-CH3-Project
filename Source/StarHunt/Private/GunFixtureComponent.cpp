// Fill out your copyright notice in the Description page of Project Settings.


#include "GunFixtureComponent.h"
#include "ItemBlueprintFunctionLibrary.h"
#include "ItemSubsystem.h"
// Sets default values for this component's properties
UGunFixtureComponent::UGunFixtureComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UGunFixtureComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGunFixtureComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGunFixtureComponent::SetFixtureType(TSet<EGunFixtureType>& FixtureTypes)
{
	if (UItemSubsystem* ItemSubsystem = Cast<UItemSubsystem>(UItemBlueprintFunctionLibrary::GetGameInstanceSubsystem()))
	{
		for (EGunFixtureType& FixtureType : FixtureTypes)
		{
			GunFixtures.Add(FixtureType, nullptr);
		}
	}
}


float UGunFixtureComponent::GetFixtursStatus() const
{
	float Damage = 0.0f;

	for (auto& ItemStateRow : GunFixtures)
	{
		Damage += ItemStateRow.Value->DamageAmount;
	}

	return Damage;
}

bool UGunFixtureComponent::AddFixture(const int32 InventoryIndex)
{
	if (UItemSubsystem* ItemSubsystem = Cast<UItemSubsystem>(UItemBlueprintFunctionLibrary::GetGameInstanceSubsystem()))
	{
		TSharedPtr<FString> ItemID = ItemSubsystem->GetInventoryItemID(EInventoryType::GunFixture, InventoryIndex);
		if (FGunFixtureItemStateRow* GunFixtureItemStateRow = ItemSubsystem->ItemDB->GetGunFixtureItemStateRow(*ItemID))
		{
			EGunFixtureType GunFixtureType = GunFixtureItemStateRow->FixtureType;
			if (GunFixtures.Contains(GunFixtureType))
			{
				if (RemoveFixture(GunFixtureType))
				{
					GunFixtures[GunFixtureType] = GunFixtureItemStateRow;
					return true;
				}

			}

		}
		
	}
	return false;
}

bool UGunFixtureComponent::RemoveFixture(EGunFixtureType FixtureType)
{
	if (UItemSubsystem* ItemSubsystem = Cast<UItemSubsystem>(UItemBlueprintFunctionLibrary::GetGameInstanceSubsystem()))
	{
		if (FGunFixtureItemStateRow* GunFixtureItemStateRow = *GunFixtures.Find(FixtureType))
		{
			if (ItemSubsystem->AddItem(GunFixtureItemStateRow->ItemID))
			{
				GunFixtures[FixtureType] = nullptr;
				return true;
			}
		}
	}
	return false;
}

