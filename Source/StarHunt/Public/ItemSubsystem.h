// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Delegates/DelegateCombinations.h"
#include "ItemDB.h"
#include "ItemSubsystem.generated.h"


struct FInventoryItem
{
	FString ItemID;
	int32 Stock;
	FInventoryItem(FString Id = "", int32 Stock = 0)
		:ItemID(Id), Stock(Stock)
	{
	}
	bool operator==(const FInventoryItem& Other) const
	{
		return ItemID == Other.ItemID;
	}
};
struct FEquipment
{
	TSharedPtr<FString> ItemID;
	TMap<EGunFixtureType, TSharedPtr<FString>> FixtureItemIDs;

	FEquipment(const FString& Id, const TSet<EGunFixtureType> FixtureItemTypes)
		:ItemID(MakeShared<FString>(Id))
	{
		for (const EGunFixtureType& FixtureItemType : FixtureItemTypes)
		{
			FixtureItemIDs.Add(FixtureItemType, nullptr);
		}
	}
	FEquipment(const FEquipment& Equipment)
		:ItemID(Equipment.ItemID), FixtureItemIDs(Equipment.FixtureItemIDs)
	{
	}
	bool operator==(const FInventoryItem& Other) const
	{
		return *ItemID == *Other.ItemID;
	}
};
/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryChange, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipmentChange, int32);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnGunFixtureChange, const int32, EGunFixtureType);

UCLASS()
class STARHUNT_API UItemSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// 아이템 관련 로직
	UItemSubsystem();
	bool AddItem(const FString& ItemId);
	bool SwapItem(EInventoryType ItemType, const int32 IndexFrom, const int32 IndexTo);
	bool RemoveItem(EInventoryType ItemType, int32 InventoryIndex);
	const TSharedPtr<FString> GetInventoryItemID(EInventoryType ItemType, int32 InventoryIndex) const;
	bool ValidAddInventory(const FString& ItemId);

	AActor* SpawnDropItem(const FString& ItemId);

	// 아이템 관련 Get
	const TMap<EInventoryType, TArray<TSharedPtr<FInventoryItem>>>* GetInventorys() const;
	const TArray<TSharedPtr<FInventoryItem>>* GetInventory(EInventoryType ItemType) const;
	const int32 GetInventoryEmptyNum(EInventoryType ItemType) const;
	const int32 GetInventoryMaxStock() const;

	//총기 관련 로직
	bool AddGunEquipment(int32 EquipmentIndex, const int32 InventoryIndex);
	bool AddGunEquipmentByDropGun(int32 EquipmentIndex, const FString& ItemID);
	bool RemoveGunEquipment(int32 EquipmentIndex);
	bool ValidAddGunEquipment(int32 EquipmentIndex, const FString& ItemId);
	bool SwapGunEquipment(const int32 IndexFrom, const int32 IndexTo);

	//총기 관련 Get
	const TArray< TSharedPtr<FEquipment>>* GetEquipments() const;
	const TSharedPtr<FString> GetEquipmentGunItemID(const int32 EquipmentIndex) const;
	
	//부착물 관련 로직
	bool AddGunFixtureEquipment(const int32 EquipmentIndex, const int32 InventoryIndex);
	bool SwapGunFixtureEquipment(const int32 EquipmentIndexFrom, const int32 EquipmentIndexTo, EGunFixtureType GunFixtureType);
	bool RemoveGunFixtureEquipment(const int32 EquipmentIndex, EGunFixtureType GunFixtureType);
	bool ValidGunFixtureEquipment(const int32 EquipmentIndex, EGunFixtureType ItemId);

	// 부착물 관련 Get
	const TMap<EGunFixtureType, TSharedPtr<FString>> GetEquipmentGunFixtureItemIDs(const int32 EquipmentIndex) const;
	const TSharedPtr<FString> GetEquipmentGunFixtureItemID(const int32 EquipmentIndex, EGunFixtureType GunFixtureType) const;


	//아이템관련 DB
	void SetItemDb(TSubclassOf<UItemDB> DB);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSystem|ItemDb")
	UItemDB* ItemDB;



	FBaseItemStateRow* GetBaseItemStateRow(const FString& ItemId);
	FHealingItemStateRow* GetHealingItemStateRow(const FString& ItemId);
	FGunItemStateRow* GetGunItemStateRow(const FString& ItemId);
	FGunFixtureItemStateRow* GetGunFixtureItemStateRow(const FString& ItemId);
	
	FOnInventoryChange OnInventoryChange;
	FOnEquipmentChange OnEquipmentChange;
	FOnGunFixtureChange OnGunFixtureChange;

private:
	TMap<EInventoryType, TArray<TSharedPtr<FInventoryItem>>> Inventorys;
	TArray< TSharedPtr<FEquipment>> Equipments;

	int32 InventoryMaxStock;
	int32 EquipmentMaxStock;
};
