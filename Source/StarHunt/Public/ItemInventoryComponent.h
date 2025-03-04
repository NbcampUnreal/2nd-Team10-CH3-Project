// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/WidgetComponent.h"
#include "ItemInventoryComponent.generated.h"

class ABaseGun;
class USphereComponent;
class ADropItemActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STARHUNT_API UItemInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemInventoryComponent();

public:
	UFUNCTION(BlueprintCallable)
	ABaseGun* GetWeapon(int32 EquipmentIndex);
	UFUNCTION(BlueprintCallable)
	void WeaponChange(int32 EquipmentIndex);
	UFUNCTION(BlueprintCallable)
	void DistoryWeapon();

	TArray<TSharedPtr<FString>*> GetNearbyItems();
	TArray<ADropItemActor*> GetNearbyItemActors();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	ABaseGun* Gun;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 CurrentEquipmentIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Components")
	USphereComponent* CollisionComponent;

	FDelegateHandle GunChangeHandler;
	TArray<TSharedPtr<FString>*> OverlappingItems;
	TArray<ADropItemActor*> OverlappingItemActors;
};


