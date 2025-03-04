// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemStateRow.h"
#include "GunFixtureComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STARHUNT_API UGunFixtureComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGunFixtureComponent();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
	void UpdateFixturesStatus();

	FGunFixtureItemStateRow* GetFixtursStatus() const;



	UFUNCTION(BlueprintCallable)
	virtual void StartItemSubsystem(int32 EquipmentIndex);
	UFUNCTION(BlueprintCallable)
	virtual void EndItemSubsystem();

	int32 CurrentEquipmentIndex;

	TMap<EGunFixtureType, FGunFixtureItemStateRow*> GunFixtures;
	TSharedPtr<FGunFixtureItemStateRow> GunFixturesStatus;

	UFUNCTION(BlueprintCallable)
	void UpdateFixtureType(int32 EquipmentIndex);
	void UpdateFixtureType(int32 EquipmentIndex, EGunFixtureType GunFixtureType);
private:
	FDelegateHandle GunFixtureChangeHandler;
};
