// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemStateRow.h"
#include "DropItemActor.generated.h"

class USphereComponent;
class UWidgetComponent;
UCLASS()
class STARHUNT_API ADropItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADropItemActor();
public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Components")
	USceneComponent* RootSceneComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Components")
	USphereComponent* CollisionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Components")
	UWidgetComponent* OverheadWidgetComponent;

	TArray<TSharedPtr<FString>> ItemIDs;
	// test
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Components")
	TArray<FString> ItemId;


	FBaseItemStateRow* GetItemState();

	void ItemDestroy();
	void ItemsUpdate();

	virtual void BeginPlay() override;
	UFUNCTION()
	void OnItemBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	UFUNCTION()
	void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
private:
	FBaseItemStateRow* ItemState;
};
