// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItemActor.h"
#include "Components/SphereComponent.h"
#include "ItemSubsystem.h"
#include "ItemBlueprintFunctionLibrary.h"
// Sets default values
ADropItemActor::ADropItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	SetRootComponent(RootSceneComponent);

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ADropItemActor::OnItemBeginOverlap);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ADropItemActor::OnItemEndOverlap);

}

FBaseItemStateRow* ADropItemActor::GetItemState()
{
	return ItemState;
}

void ADropItemActor::ItemDestroy()
{
	Destroy();
}

void ADropItemActor::BeginPlay()
{
	Super::BeginPlay();
}

void ADropItemActor::OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		if (UItemSubsystem* ItemSubSystem = Cast<UItemSubsystem>(UItemBlueprintFunctionLibrary::GetGameInstanceSubsystem()))
		{
			if (ItemSubSystem->AddItem(ItemId))
			{
				ItemDestroy();
			}
		}

	}
}

void ADropItemActor::OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


