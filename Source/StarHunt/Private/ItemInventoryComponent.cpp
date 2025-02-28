// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInventoryComponent.h"
#include "ItemBlueprintFunctionLibrary.h"
#include "BaseGun.h"
#include "DropItemActor.h"
// Sets default values for this component's properties
UItemInventoryComponent::UItemInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}



AActor* UItemInventoryComponent::GetWeapon(int32 EquipmentIndex)
{
	if (UWorld* World = GetWorld())
	{
		if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
		{
			if (TSharedPtr<FString> ItemID = ItemSubsystem->GetEquipmentGunItemID(EquipmentIndex))
			{
				if (FGunItemStateRow* GunItemStateRow = ItemSubsystem->GetGunItemStateRow(*ItemID))
				{
					if (UClass* LoadedActorClass = GunItemStateRow->GunSoftClass.LoadSynchronous())
					{
						ABaseGun* Gun = World->SpawnActor<ABaseGun>(LoadedActorClass);
						Gun->SetAbility(GunItemStateRow);
						return Gun;
					}
				}
			}
		}
	}
	return nullptr;
}

