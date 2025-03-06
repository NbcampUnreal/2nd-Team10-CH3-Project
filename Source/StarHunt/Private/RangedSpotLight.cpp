#include "RangedSpotLight.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

ARangedSpotLight::ARangedSpotLight()
{
    PrimaryActorTick.bCanEverTick = true;
    SpotLightComp = CreateDefaultSubobject<USpotLightComponent>(TEXT("TrackingSpotLight"));
    RootComponent = SpotLightComp;
    //bIsLightActive = false;

}

void ARangedSpotLight::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsLightActive && TargetActor)
    {
        // 플레이어 추적 설정
        // ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
        // if (!Player) return;
        // FVector PlayerLocation = Player->GetActorLocation();

        // 위치 계산
        FVector MyLocation = GetActorLocation();
        FVector TargetLocation = TargetActor->GetActorLocation();

        // 타겟을 바라보는 회전값 계산
        FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(MyLocation, TargetLocation);

        // 위치는 그대로 두고, 회전만 적용
        SetActorRotation(LookAtRot);
    }
    else if(bIsLightActive && !TargetActor)
    {
        // 플레이어 추적 설정
        ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

        if (!Player) return;

        // 위치 계산
        FVector MyLocation = GetActorLocation();
        FVector PlayerLocation = Player->GetActorLocation();

        // 타겟을 바라보는 회전값 계산
        FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(MyLocation, PlayerLocation);

        // 위치는 그대로 두고, 회전만 적용
        SetActorRotation(LookAtRot);
    }
}

