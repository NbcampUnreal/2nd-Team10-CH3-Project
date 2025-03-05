// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInventoryComponent.h"
#include "ItemBlueprintFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "BaseGun.h"
#include "GameFramework/Character.h"
#include "DropItemActor.h"
#include "ActivateItem.h"
#include "WraithPlayerController.h"

// Sets default values for this component's properties
UItemInventoryComponent::UItemInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
	CurrentEquipmentIndex = 0;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	ActivateItem = CreateDefaultSubobject<UActivateItem>(TEXT("ActivateItem"));
}


void UItemInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner())
	{
		CollisionComponent->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		GunChangeHandler = ItemSubsystem->OnEquipmentChange.AddUObject(this, &UItemInventoryComponent::WeaponChange);
	}
}

void UItemInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		ItemSubsystem->OnEquipmentChange.Remove(GunChangeHandler);
	}
}

void UItemInventoryComponent::UseQuickSlotIem(int32 QuickSlotIndex)
{
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		if (TSharedPtr<FInventoryItem> InventoryItem = ItemSubsystem->GetQuickSlotPointInventoryIndex(QuickSlotIndex).Pin())
		{
			if (IActivateItemInterface* ActivateItemInterface = ActivateItem->GetActivateActionClass())
			{
				if (ItemSubsystem->RemoveItem(EInventoryType::Consumable, InventoryItem->InventoryIndex))
				{
					ActivateItemInterface->SetStatus(InventoryItem->ItemID);
					ActivateItemInterface->ActivateItem(GetOwner());
					ItemSubsystem->OnQuickSlotChange.Broadcast(QuickSlotIndex);
				}
				// 소모품만 생각, 소모품도 아닐경우는 나중에 고려
				
			}
		}
	}
}



ABaseGun* UItemInventoryComponent::GetWeapon(int32 EquipmentIndex)
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
						CurrentEquipmentIndex = EquipmentIndex;
						Gun = World->SpawnActor<ABaseGun>(LoadedActorClass);
						if (Gun)
						{
							Gun->StartItemSubsystem(EquipmentIndex, GunItemStateRow);
						}
					}
				}
			}
		}
	}

	return Gun;
}

TSubclassOf<ABaseGun> UItemInventoryComponent::GetWeaponClass(int32 EquipmentIndex)
{
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		if (TSharedPtr<FString> ItemID = ItemSubsystem->GetEquipmentGunItemID(EquipmentIndex))
		{
			if (FGunItemStateRow* GunItemStateRow = ItemSubsystem->GetGunItemStateRow(*ItemID))
			{
				if (UClass* LoadedActorClass = GunItemStateRow->GunSoftClass.LoadSynchronous())
				{
					return LoadedActorClass->StaticClass();
				}
			}
		}
	}

	return nullptr;
}

FGunItemStateRow* UItemInventoryComponent::GetWeaponStatus(int32 EquipmentIndex)
{
	if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
	{
		if (TSharedPtr<FString> ItemID = ItemSubsystem->GetEquipmentGunItemID(EquipmentIndex))
		{
			if (FGunItemStateRow* GunItemStateRow = ItemSubsystem->GetGunItemStateRow(*ItemID))
			{
				return GunItemStateRow;
			}
		}
	}
	return nullptr;
}

void UItemInventoryComponent::SetWeapon(ABaseGun* BaseGun)
{
	Gun = BaseGun;
}


void UItemInventoryComponent::WeaponChange(int32 EquipmentIndex)
{
	if (CurrentEquipmentIndex == EquipmentIndex)
	{
		GetWeapon(EquipmentIndex);
	}
}

void UItemInventoryComponent::DistoryWeapon()
{
	if (Gun)
	{
		Gun->Destroyed();
	}
}

TArray<TSharedPtr<FString>*> UItemInventoryComponent::GetNearbyItems()
{
	TArray<AActor*> OverlappingActors;
	OverlappingItems.Empty();
	if (CollisionComponent)
	{
		CollisionComponent->GetOverlappingActors(OverlappingActors);

		for (AActor* Actor : OverlappingActors)
		{
			if (ADropItemActor* DropItemActor = Cast<ADropItemActor>(Actor))
			{
				for (auto& ItemID : DropItemActor->ItemIDs)
				{
					if (ItemID.IsValid())
					{
						OverlappingItems.Add(&ItemID);
					}
				}
			}
		}
	}
	return OverlappingItems;
}

TArray<ADropItemActor*> UItemInventoryComponent::GetNearbyItemActors()
{
	TArray<AActor*> OverlappingActors;
	OverlappingItemActors.Empty();
	if (CollisionComponent)
	{
		CollisionComponent->GetOverlappingActors(OverlappingActors);

		for (AActor* Actor : OverlappingActors)
		{
			if (ADropItemActor* DropItemActor = Cast<ADropItemActor>(Actor))
			{
				OverlappingItemActors.Add(DropItemActor);
			}
		}
	}
	return OverlappingItemActors;
}



