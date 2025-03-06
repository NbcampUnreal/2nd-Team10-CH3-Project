// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class STARHUNT_API UDamageSystem
{
public:
	static void ApplyDamage(AActor* DamagedActor, float Damage, AController* EventInstigator, AActor* DamageCauser, const FHitResult& SweepResult);

private:
	static bool IsHeadshot(const FHitResult& HitResult);
};