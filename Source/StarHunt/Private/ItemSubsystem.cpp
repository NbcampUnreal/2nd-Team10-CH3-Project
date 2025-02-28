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

				RemoveItem(EInventoryType::Gun, InventoryIndex);
				Equipments[EquipmentIndex] = MakeShared<FEquipment>(FEquipment(*GunItemID, GunItemStateRow->FixtureTypes));
				
				OnEquipmentChange.Broadcast(EquipmentIndex);
				OnInventoryChange.Broadcast(InventoryIndex);


				for (EGunFixtureType GunFixtureType : GunItemStateRow->FixtureTypes)
				{
					OnGunFixtureChange.Broadcast(EquipmentIndex, GunFixtureType);
				}
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
			TSet<EGunFixtureType> FixtureTypes;
			for (auto& FixtureItemPair : Equipments[EquipmentIndex]->FixtureItemIDs)
			{
				if (FixtureItemPair.Value != nullptr)
				{
					AddItem(*FixtureItemPair.Value);
				}
				FixtureTypes.Add(FixtureItemPair.Key);
				FixtureItemPair.Value = nullptr;
			}
			if (AddItem(*Equipments[EquipmentIndex]->ItemID))
			{
				Equipments[EquipmentIndex] = nullptr;
				OnEquipmentChange.Broadcast(EquipmentIndex);
				for (EGunFixtureType GunFixtureType : FixtureTypes)
				{
					OnGunFixtureChange.Broadcast(EquipmentIndex, GunFixtureType);
				}
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

bool UItemSubsystem::AddGunFixtureEquipment(const int32 EquipmentIndex, const int32 InventoryIndex)
{
	if (Equipments.IsValidIndex(EquipmentIndex) && Inventorys[EInventoryType::GunFixture].IsValidIndex(InventoryIndex))
	{
		if (TSharedPtr<FString> ItemID = GetInventoryItemID(EInventoryType::GunFixture, InventoryIndex))
		{

			if (ItemID.IsValid())
			{

				if (FGunFixtureItemStateRow* GunFixtureItemStateRow = GetGunFixtureItemStateRow(*ItemID))
				{

					if (Equipments[EquipmentIndex]->FixtureItemIDs.Contains(GunFixtureItemStateRow->FixtureType))
					{
						TSharedPtr<FString>* EquipedGunFixtureItemId = Equipments[EquipmentIndex]->FixtureItemIDs.Find(GunFixtureItemStateRow->FixtureType);

						RemoveItem(EInventoryType::GunFixture, InventoryIndex);
						Equipments[EquipmentIndex]->FixtureItemIDs[GunFixtureItemStateRow->FixtureType] = MakeShared<FString>(*ItemID); 

						OnGunFixtureChange.Broadcast(EquipmentIndex, GunFixtureItemStateRow->FixtureType);
						OnInventoryChange.Broadcast(InventoryIndex);

						if (EquipedGunFixtureItemId)
						{
							return AddItem(**EquipedGunFixtureItemId);
						}
					}
				}
			}
		}
	}
	return false;
}

bool UItemSubsystem::SwapGunFixtureEquipment(const int32 EquipmentIndexFrom, const int32 EquipmentIndexTo, EGunFixtureType GunFixtureType)
{
	if (EquipmentIndexFrom != EquipmentIndexTo &&
		Equipments.IsValidIndex(EquipmentIndexFrom) &&
		Equipments.IsValidIndex(EquipmentIndexTo))
	{
		if (Equipments[EquipmentIndexFrom].IsValid() && Equipments[EquipmentIndexTo].IsValid())
		{
			if (TSharedPtr<FString>* ItemIDFrom = Equipments[EquipmentIndexFrom]->FixtureItemIDs.Find(GunFixtureType))
			{
				if (TSharedPtr<FString>* ItemIDTo = Equipments[EquipmentIndexTo]->FixtureItemIDs.Find(GunFixtureType))
				{
					TSharedPtr<FString> Temp = *ItemIDTo;

					*ItemIDTo = *ItemIDFrom;
					*ItemIDFrom = Temp;
					OnGunFixtureChange.Broadcast(EquipmentIndexFrom, GunFixtureType);
					OnGunFixtureChange.Broadcast(EquipmentIndexTo, GunFixtureType);
				}
			}
		}
	}
	return false;
}

bool UItemSubsystem::RemoveGunFixtureEquipment(const int32 EquipmentIndex, EGunFixtureType GunFixtureType)
{
	if (Equipments.IsValidIndex(EquipmentIndex))
	{
		if (Equipments[EquipmentIndex].IsValid())
		{

			if (TSharedPtr<FString>* ItemID = Equipments[EquipmentIndex]->FixtureItemIDs.Find(GunFixtureType))
			{

				if (ItemID->IsValid() && AddItem(**ItemID))
				{

					*ItemID = nullptr;
					OnGunFixtureChange.Broadcast(EquipmentIndex, GunFixtureType);
					return true;
				}
			}
		}
	}
	return false;
}
bool UItemSubsystem::ValidGunFixtureEquipment(const int32 EquipmentIndex, EGunFixtureType GunFixtureType)
{
	if (Equipments.IsValidIndex(EquipmentIndex))
	{
		if (Equipments[EquipmentIndex].IsValid())
		{
			if (Equipments[EquipmentIndex]->FixtureItemIDs.Contains(GunFixtureType))
			{
				return true;
			}
		}
	}
	return false;
}

const TSharedPtr<FString> UItemSubsystem::GetEquipmentGunFixtureItemID(const int32 EquipmentIndex, EGunFixtureType GunFixtureType) const
{
	if (Equipments.IsValidIndex(EquipmentIndex))
	{

		if (TSharedPtr<FEquipment> Equipment = Equipments[EquipmentIndex])
		{

			if (Equipment.IsValid())
			{
				if (TSharedPtr<FString>* ItemID = Equipment->FixtureItemIDs.Find(GunFixtureType))
				{
					if (ItemID->IsValid())
					{
						return *ItemID;
					}

				}
			}
		}
	}
	return nullptr;
}

const TArray<TSharedPtr<FEquipment>>* UItemSubsystem::GetEquipments() const
{
	return &Equipments;
}

const TMap<EGunFixtureType, TSharedPtr<FString>> UItemSubsystem::GetEquipmentGunFixtureItemIDs(const int32 EquipmentIndex) const
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
