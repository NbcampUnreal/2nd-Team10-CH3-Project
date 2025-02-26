#include "You.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AYou::AYou()
{
    PrimaryActorTick.bCanEverTick = true;

    MaxHealth = 100.0f;
    Health = MaxHealth;
}

void AYou::BeginPlay()
{
    Super::BeginPlay();
}

void AYou::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AYou::TakeDamage(float DamageAmount)
{
    Health -= DamageAmount;
    UE_LOG(LogTemp, Warning, TEXT("HP: %.0f / %.0f"), Health, MaxHealth);
    if (Health <= 0)
    {
        Die();
    }
}

void AYou::Die()
{
    UE_LOG(LogTemp, Error, TEXT("Enemy : DEAD"));
    Destroy();
}
