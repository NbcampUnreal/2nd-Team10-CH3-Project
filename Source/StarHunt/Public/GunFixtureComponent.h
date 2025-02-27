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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetFixtureType(TSet<FString>& Type);

	float GetFixtursStatus() const;
	bool AddFixture(const int32 InventoryIndex);
	bool RemoveFixture(const FString& FixtureType);
	TMap<FString, FGunFixtureItemStateRow*> GunFixtures;
};
