// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunFixtureComponent.h"
#include "TestGunActor.generated.h"

UCLASS()
class STARHUNT_API ATestGunActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestGunActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGunFixtureComponent* GunFixtureComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* RootSceneComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* GunDataTable;
};
