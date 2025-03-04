// Fill out your copyright notice in the Description page of Project Settings.


#include "AimingSystem.h"

UAimingSystem::UAimingSystem()
{
    bDebug = false;
}

FVector UAimingSystem::GetTargetLocation(APlayerController* PlayerController)
{
    if (!PlayerController)
    {
        return FVector::ZeroVector;
    }

    FVector CameraLocation;
    FRotator CameraRotation;

    // 플레이어 카메라 위치 & 방향 가져오기
    PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

    FVector TraceStart = CameraLocation;
    FVector TraceEnd = TraceStart + (CameraRotation.Vector() * 5000.0f);

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(PlayerController->GetPawn());
    UWorld* World = PlayerController->GetWorld();

    bool bHit = World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

    // 디버그 모드
    if (bDebug)
    {
        // 라인 트레이스 경로
        DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, false, 2.0f, 0, 2.0f);

        if (bHit)
        {
            // 충돌 지점
            DrawDebugSphere(World, HitResult.ImpactPoint, 10.0f, 12, FColor::Green, false, 2.0f);
        }
    }

    return bHit ? HitResult.ImpactPoint : TraceEnd;
}

FRotator UAimingSystem::GetTargetRotation(APlayerController* PlayerController, const FVector& SpawnLocation)
{
    if (!PlayerController)
    {
        return FRotator::ZeroRotator;
    }
    FVector Target = GetTargetLocation(PlayerController);

    return (Target - SpawnLocation).Rotation();
}

void UAimingSystem::SetDebugMode(bool bIsDebugmode)
{
    bDebug = bIsDebugmode;
}
