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

float AYou::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    Health = FMath::Clamp(Health - ActualDamage, 0.0f, MaxHealth);
    if (Health <= 0.0f)
    {
        Die();
    }
    return ActualDamage;
}

void AYou::Die()
{
    UE_LOG(LogTemp, Error, TEXT("Enemy : DEAD"));
    Destroy();
}
