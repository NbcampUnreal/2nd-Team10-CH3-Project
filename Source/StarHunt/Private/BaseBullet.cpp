// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseBullet::ABaseBullet()
{
    PrimaryActorTick.bCanEverTick = false;

    // 씬 컴포넌트 생성
    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    // 충돌 컴포넌트 생성 및 설정
    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    Collision->SetupAttachment(Scene);

    // 스태틱 메시 생성
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetupAttachment(Collision);


    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->InitialSpeed = 5000.f;
    ProjectileMovement->MaxSpeed = 5000.f;
    ProjectileMovement->ProjectileGravityScale = 0.0f;

    BulletDamage = 0.0f;

    InitialLifeSpan = 3.0f;

    Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseBullet::OnOverlap);
}

void ABaseBullet::SetBulletDamage(float NewDamage)
{
    BulletDamage = NewDamage;
}

float ABaseBullet::GetBulletDamage() const
{
    return BulletDamage;
}

void ABaseBullet::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult)
{    
    if (!OtherActor || OtherActor->IsA(ABaseBullet::StaticClass())) return;
    if (!OtherActor->CanBeDamaged()) return;

    // ApplyDamage 인수로 들어갈 컨트롤러 변수
    AController* PlayerController = GetWorld()->GetFirstPlayerController();
    
    if (PlayerController)
    {
    UGameplayStatics::ApplyDamage(OtherActor, BulletDamage, PlayerController, this, UDamageType::StaticClass());
    }
    
    Destroy();
}
