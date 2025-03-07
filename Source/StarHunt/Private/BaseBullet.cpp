// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "DamageSystem.h"

// Sets default values
ABaseBullet::ABaseBullet()
{
    PrimaryActorTick.bCanEverTick = false;

    // 충돌 컴포넌트 생성 및 설정
    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Collision->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    Collision->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
    Collision->SetNotifyRigidBodyCollision(true);

    SetRootComponent(Collision);

    // 스태틱 메시 생성
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetupAttachment(Collision);


    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->InitialSpeed = 5000.f;
    ProjectileMovement->MaxSpeed = 5000.f;
    ProjectileMovement->ProjectileGravityScale = 0.0f;
    ProjectileMovement->bAutoActivate = true;

    BulletDamage = 0.0f;

    InitialLifeSpan = 3.0f;

    Collision->OnComponentHit.AddDynamic(this, &ABaseBullet::OnHit);
}

void ABaseBullet::BeginPlay()
{
    Super::BeginPlay();

    //Collision->BodyInstance.SetUseCCD(true);
}

void ABaseBullet::SetBulletDamage(float NewDamage)
{
    BulletDamage = NewDamage;
}

float ABaseBullet::GetBulletDamage() const
{
    return BulletDamage;
}

UProjectileMovementComponent* ABaseBullet::GetProjectileComp()
{
    return ProjectileMovement;
}


void ABaseBullet::OnHit(
    UPrimitiveComponent* HitComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (!OtherActor || OtherActor->IsA(ABaseBullet::StaticClass())) return;
    if (!OtherActor->CanBeDamaged()) return;

    // ApplyDamage 인수로 들어갈 컨트롤러 변수
    AController* PlayerController = GetWorld()->GetFirstPlayerController();
    

    if (PlayerController)
    {
        UDamageSystem::ApplyDamage(OtherActor, BulletDamage, PlayerController, this, Hit);
    }

    Destroy();
}