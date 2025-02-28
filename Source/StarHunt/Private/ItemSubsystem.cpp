// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSubsystem.h"

UItemSubsystem::UItemSubsystem()
{
	ItemDB = CreateDefaultSubobject<UItemDB>(TEXT("ItemDB"));
	InventoryMaxStock = 16;
	EquipmentMaxStock = 3;
}

void UItemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	
	Inventorys.Add(EInventoryType::Gun, TArray<TSharedPtr<FInventoryItem>>());
	Inventorys.Add(EInventoryType::GunFixture, TArray<TSharedPtr<FInventoryItem>>());
	Inventorys.Add(EInventoryType::Consumable, TArray<TSharedPtr<FInventoryItem>>());

	for (auto& InventoryPair : Inventorys)
	{
		InventoryPair.Value.SetNum(InventoryMaxStock);
	}

	Equipments.SetNum(EquipmentMaxStock);
}


void UItemSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

FBaseItemStateRow* UItemSubsystem::GetBaseItemStateRow(const FString& ItemId)
{
	if (ItemDB)
	{
		return ItemDB->GetBaseItemStateRow(ItemId);
	}
	return nullptr;
}

FHealingItemStateRow* UItemSubsystem::GetHealingItemStateRow(const FString& ItemId)
{
	if (ItemDB)
	{
		return ItemDB->GetHealingItemStateRow(ItemId);
	}
	return nullptr;
}

FGunItemStateRow* UItemSubsystem::GetGunItemStateRow(const FString& ItemId)
{
	if (ItemDB)
	{
		return ItemDB->GetGunItemStateRow(ItemId);
	}
	return nullptr;
}

FGunFixtureItemStateRow* UItemSubsystem::GetGunFixtureItemStateRow(const FString& ItemId)
{
	if (ItemDB)
	{
		return ItemDB->GetGunFixtureItemStateRow(ItemId);
	}
	return nullptr;
}

bool UItemSubsystem::AddItem(const FString& ItemId)
{
	if (FBaseItemStateRow* BaseItemStateRow = ItemDB->GetBaseItemStateRow(ItemId))
	{
		if (TArray<TSharedPtr<FInventoryItem>>* Inventory = Inventorys.Find(BaseItemStateRow->ItemType))
		{
			int32 EmptyIndex = -1;
			for (int32 InventoryItemIndex = 0; InventoryItemIndex < InventoryMaxStock; InventoryItemIndex++)
			{
				TSharedPtr<FInventoryItem> InventoryItem = (*Inventory)[InventoryItemIndex];
				
				if (!InventoryItem.IsValid() && EmptyIndex == -1)
				{
					EmptyIndex = InventoryItemIndex;
				}
				else if (InventoryItem.IsValid() && InventoryItem->ItemID == ItemId && InventoryItem->Stock < BaseItemStateRow->StockMax)
				{
					InventoryItem->Stock++;
					OnInventoryChange.Broadcast(InventoryItemIndex);
					return true;
				}
			}
			if (EmptyIndex != -1)
			{
				(*Inventory)[EmptyIndex] = MakeShared<FInventoryItem>(FInventoryItem(ItemId, 1));
				OnInventoryChange.Broadcast(EmptyIndex);
				return true;
			}
		}
	}
	
	return false;
}

bool UItemSubsystem::SwapItem(EInventoryType ItemType, const int32 IndexFrom, const int32 IndexTo)
{
	if (TArray<TSharedPtr<FInventoryItem>>* TargetInventory = Inventorys.Find(ItemType))
	{
		if (TargetInventory->IsValidIndex(IndexFrom) && TargetInventory->IsValidIndex(IndexTo))
		{
			TargetInventory->Swap(IndexFrom, IndexTo);
			OnInventoryChange.Broadcast(IndexFrom);
			OnInventoryChange.Broadcast(IndexTo);
			return true;
		}
	}
	return false;
}

bool UItemSubsystem::SwapGunEquipment(const int32 IndexFrom, const int32 IndexTo)
{
	if (Equipments.IsValidIndex(IndexFrom) && Equipments.IsValidIndex(IndexTo))
	{
		Equipments.Swap(IndexFrom, IndexTo);
		OnEquipmentChange.Broadcast(IndexFrom);
		OnEquipmentChange.Broadcast(IndexTo);
		return true;
	}
	return false;
}

bool UItemSubsystem::RemoveItem(EInventoryType ItemType, int32 InventoryIndex)
{
	if (TArray<TSharedPtr<FInventoryItem>>* TargetInventory = Inventorys.Find(ItemType))
	{
		if (TargetInventory->IsValidIndex(InventoryIndex))
		{
			if ((*TargetInventory)[InventoryIndex].IsValid())
			{
				if (FInventoryItem* Item = (*TargetInventory)[InventoryIndex].Get())
				{
					Item->Stock--;
					if (Item->Stock <= 0)
					{
						(*TargetInventory)[InventoryIndex] = nullptr;

					}
					OnInventoryChange.Broadcast(InventoryIndex);
					return true;
				}

			}
		}
	}
	return false;
}

const TSharedPtr<FString> UItemSubsystem::GetInventoryItemID(EInventoryType ItemType, int32 InventoryIndex) const
{
	if (const TArray<TSharedPtr<FInventoryItem>>* TargetInventory = Inventorys.Find(ItemType))
	{
		if (TargetInventory->IsValidIndex(InventoryIndex))
		{
			TSharedPtr<FInventoryItem> Item = (*TargetInventory)[InventoryIndex];
			if (Item.IsValid())
			{
				return MakeShared<FString>(Item->ItemID);
			}
		}
	}
	return nullptr;
}

bool UItemSubsystem::ValidAddInventory(const FString& ItemId)
{
	if (FBaseItemStateRow* BaseItemStateRow = ItemDB->GetBaseItemStateRow(ItemId))
	{
		if (TArray<TSharedPtr<FInventoryItem>>* Inventory = Inventorys.Find(BaseItemStateRow->ItemType))
		{
			if (GetInventoryEmptyNum(BaseItemStateRow->ItemType) > 0)
			{
				return true;
			}

			for (int32 InventoryItemIndex = 0; InventoryItemIndex < InventoryMaxStock; InventoryItemIndex++)
			{
				TSharedPtr<FInventoryItem> InventoryItem = (*Inventory)[InventoryItemIndex];

				if (InventoryItem->ItemID == ItemId && InventoryItem->Stock < BaseItemStateRow->StockMax)
				{
					return true;
				}
			}
		}
	}
	return false;
}


AActor* UItemSubsystem::SpawnDropItem(const FString& ItemId)
{
	return nullptr;
}

const TMap<EInventoryType, TArray<TSharedPtr<FInventoryItem>>>* UItemSubsystem::GetInventorys() const
{
	return &Inventorys;
}

const TArray<TSharedPtr<FInventoryItem>>* UItemSubsystem::GetInventory(EInventoryType ItemType) const
{
	return Inventorys.Find(ItemType);
}

const int32 UItemSubsystem::GetInventoryEmptyNum(EInventoryType ItemType) const
{
	return InventoryMaxStock - (*GetInventory(ItemType)).Num();
}

const int32 UItemSubsystem::GetInventoryMaxStock() const
{
	return InventoryMaxStock;
}


bool UItemSubsystem::AddGunEquipment(int32 EquipmentIndex, int32 InventoryIndex)
{
	if (TSharedPtr<FString> GunItemID = GetInventoryItemID(EInventoryType::Gun, InventoryIndex))
	{
		if (FGunItemStateRow* GunItemStateRow = ItemDB->GetGunItemStateRow(*GunItemID))
		{
			if (Equipments.IsValidIndex(EquipmentIndex))
			{
				if (Equipments[EquipmentIndex])
				{
					//부착물 제거
					for (auto& FixtureItemPair : Equipments[EquipmentIndex]->FixtureItemIDs)
					{
						if (FixtureItemPair.Value != nullptr)
						{
							AddItem(*FixtureItemPair.Value);
						}
					}

					//제거전 잠시보관
					if (TSharedPtr<FString> EquipedGunItemID = Equipments[EquipmentIndex]->ItemID)
					{
						/*AddItem(*EquipedGunItemID);*/
					}

				}

				OnEquipmentChange.Broadcast(EquipmentIndex);
				OnInventoryChange.Broadcast(InventoryIndex);
				Equipments[EquipmentIndex] = MakeShared<FEquipment>(FEquipment(*GunItemID, GunItemStateRow->FixtureTypes));
				return true;

			}

		}

	}
	

	return false;
}

bool UItemSubsystem::RemoveGunEquipment(int32 EquipmentIndex)
{
	if (Equipments.IsValidIndex(EquipmentIndex))
	{
		if (Equipments[EquipmentIndex].IsValid())
		{
			for (auto& FixtureItemPair : Equipments[EquipmentIndex]->FixtureItemIDs)
			{
				if (FixtureItemPair.Value != nullptr)
				{
					AddItem(*FixtureItemPair.Value);
				}
				FixtureItemPair.Value = nullptr;
			}
			if (AddItem(*Equipments[EquipmentIndex]->ItemID))
			{
				Equipments[EquipmentIndex] = nullptr;
				OnEquipmentChange.Broadcast(EquipmentIndex);
				return true;
			}
		}
	}
	return false;
}

bool UItemSubsystem::ValidAddGunEquipment(int32 EquipmentIndex, const FString& ItemId)
{
	if (FGunItemStateRow* GunItemStateRow = ItemDB->GetGunItemStateRow(ItemId))
	{
		if (Equipments.IsValidIndex(EquipmentIndex))
		{
			return true;
		}
	}
	return false;
}

bool UItemSubsystem::AddGunFixtureEquipment(int32 EquipmentIndex, const FString& ItemId)
{
	if (Equipments.IsValidIndex(EquipmentIndex))
	{

		FGunFixtureItemStateRow* GunFixtureItemStateRow = ItemDB->GetGunFixtureItemStateRow(ItemId);
		if (Equipments[EquipmentIndex]->FixtureItemIDs.Contains(GunFixtureItemStateRow->FixtureType))
		{
			if (TSharedPtr<FString> EquipedGunFixtureItemId = *Equipments[EquipmentIndex]->FixtureItemIDs.Find(GunFixtureItemStateRow->FixtureType))
			{
				if (AddItem(*EquipedGunFixtureItemId))
				{
					Equipments[EquipmentIndex]->FixtureItemIDs[GunFixtureItemStateRow->FixtureType] = MakeShared<FString>(ItemId);
					return true;
				}
			}

		}
	}
	return false;
}

bool UItemSubsystem::RemoveGunFixtureEquipment(int32 EquipmentIndex, const FString& ItemId)
{
	if (Equipments.IsValidIndex(EquipmentIndex))
	{
		if (Equipments[EquipmentIndex].IsValid())
		{
			for (auto& FixtureItemPair : Equipments[EquipmentIndex]->FixtureItemIDs)
			{
				if (*FixtureItemPair.Value == ItemId)
				{
					if (AddItem(ItemId))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}
bool UItemSubsystem::ValidAddGunFixtureEquipment(int32 EquipmentIndex, const FString& ItemId)
{
	if (Equipments.IsValidIndex(EquipmentIndex))
	{

		FGunFixtureItemStateRow* GunFixtureItemStateRow = ItemDB->GetGunFixtureItemStateRow(ItemId);
		if (Equipments[EquipmentIndex]->FixtureItemIDs.Contains(GunFixtureItemStateRow->FixtureType))
		{
			if (TSharedPtr<FString> EquipedGunFixtureItemId = *Equipments[EquipmentIndex]->FixtureItemIDs.Find(GunFixtureItemStateRow->FixtureType))
			{
				return true;
			}
		}
	}
	return false;
}

const TArray<TSharedPtr<FEquipment>>* UItemSubsystem::GetEquipments() const
{
	return &Equipments;
}

const TMap<EGunFixtureType, TSharedPtr<FString>> UItemSubsystem::GetEquipmentGunFixtureItemID(const int32 EquipmentIndex) const
{
	if (Equipments.IsValidIndex(EquipmentIndex))
	{
		return Equipments[EquipmentIndex]->FixtureItemIDs;
	}
	return TMap<EGunFixtureType, TSharedPtr<FString>>();
}

const TSharedPtr<FString> UItemSubsystem::GetEquipmentGunItemID(const int32 EquipmentIndex) const
{
	if (Equipments.IsValidIndex(EquipmentIndex) && Equipments[EquipmentIndex])
	{
		return Equipments[EquipmentIndex]->ItemID;
	}
	return nullptr;
}

void UItemSubsystem::SetItemDb(TSubclassOf<UItemDB> DB)
{
	ItemDB = NewObject<UItemDB>(this, DB);
	if (ItemDB)
	{

	}
}
