// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageSystem.h"
#include "Kismet/GameplayStatics.h"

void UDamageSystem::ApplyDamage(AActor* DamagedActor, float Damage, AController* EventInstigator, AActor* DamageCauser, const FHitResult& SweepResult)
{
    float TotalDamage = Damage;

    if (IsHeadshot(SweepResult))
    {
        TotalDamage *= 2;
    }

    UGameplayStatics::ApplyDamage(DamagedActor, TotalDamage, EventInstigator, DamageCauser, UDamageType::StaticClass());
}

bool UDamageSystem::IsHeadshot(const FHitResult& HitResult)
{
    // 머리에 맞았을 때 true
    if (HitResult.BoneName == FName("head"))
    {
        return true;
    }
    return false;
}