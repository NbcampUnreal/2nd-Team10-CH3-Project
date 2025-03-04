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
	CurrentEquipmentIndex = 0;
	// ...
}

//총이 변화할때마다 Actor및 GunFixtureComponent가 새로 생성하기 때문에 총에 변화는 곧 GunFixtureComponent가 소멸하고 생성한다

void UGunFixtureComponent::StartItemSubsystem(int32 EquipmentIndex)
{
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		GunFixtureChangeHandler = ItemSubsystem->OnGunFixtureChange.AddUObject(this, &UGunFixtureComponent::UpdateFixtureType);
		GunFixtureChangeHandler = ItemSubsystem->OnEquipmentChange.AddUObject(this, &UGunFixtureComponent::UpdateFixtureType);

	}
	CurrentEquipmentIndex = EquipmentIndex;
	UpdateFixtureType(CurrentEquipmentIndex);
}

void UGunFixtureComponent::EndItemSubsystem()
{
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		ItemSubsystem->OnEquipmentChange.Remove(GunFixtureChangeHandler);
	}
}

void UGunFixtureComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	EndItemSubsystem();
}

void UGunFixtureComponent::UpdateFixtureType(int32 EquipmentIndex)
{
	if (CurrentEquipmentIndex == EquipmentIndex)
	{
		GunFixtures.Empty();
		if (UItemSubsystem* ItemSubsystem = Cast<UItemSubsystem>(UItemBlueprintFunctionLibrary::GetGameInstanceSubsystem()))
		{
			for (auto& GunFixturesPair : ItemSubsystem->GetEquipmentGunFixtureItemIDs(EquipmentIndex))
			{
				if (GunFixturesPair.Value.IsValid())
				{
					if (FGunFixtureItemStateRow* GunFixtureItemStateRow = ItemSubsystem->GetGunFixtureItemStateRow(*GunFixturesPair.Value))
					{
						GunFixtures.Add(GunFixturesPair.Key, GunFixtureItemStateRow);
					}
				}
				else
				{
					GunFixtures.Add(GunFixturesPair.Key, nullptr);
				}
			}
			UpdateFixturesStatus();
		}
	}
}

void UGunFixtureComponent::UpdateFixtureType(int32 EquipmentIndex, EGunFixtureType GunFixtureType)
{
	if (CurrentEquipmentIndex == EquipmentIndex)
	{
		if (UItemSubsystem* ItemSubsystem = Cast<UItemSubsystem>(UItemBlueprintFunctionLibrary::GetGameInstanceSubsystem()))
		{
			for (auto& ItemStateRow : GunFixtures)
			{
				if (GunFixtureType == ItemStateRow.Key)
				{
					if (ItemSubsystem->ValidGunFixtureEquipment(EquipmentIndex, GunFixtureType))
					{

						if (TSharedPtr<FString> ItemID = *ItemSubsystem->GetEquipmentGunFixtureItemIDs(EquipmentIndex).Find(GunFixtureType))
						{

							if (ItemID.IsValid())
							{

								GunFixtures[GunFixtureType] = ItemSubsystem->GetGunFixtureItemStateRow(*ItemID);
							}
						}
						else
						{
							GunFixtures[GunFixtureType] = nullptr;
						}
					}
				}
			}

			UpdateFixturesStatus();
		}
	}
}

void UGunFixtureComponent::UpdateFixturesStatus()
{
	GunFixturesStatus = MakeShared<FGunFixtureItemStateRow>();
	for (auto& ItemStateRow : GunFixtures)
	{
		if (ItemStateRow.Value)
		{
			*GunFixturesStatus += *ItemStateRow.Value;
		}
	}
}


FGunFixtureItemStateRow* UGunFixtureComponent::GetFixtursStatus() const
{
	if (GunFixturesStatus.IsValid())
	{
		return GunFixturesStatus.Get();
	}
	return nullptr;
}