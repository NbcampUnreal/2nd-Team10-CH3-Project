#include "ASpawner.h"
#include "Engine/World.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
AASpawner::AASpawner()
{
    PrimaryActorTick.bCanEverTick = false;

    // 트리거 볼륨 생성
    TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
    RootComponent = TriggerVolume;

    // 콜리전 응답 설정(OverlapAllDynamic 등 필요에 맞게)
    TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerVolume->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
    TriggerVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    TriggerVolume->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);

    bHasTriggered = false;
    CurrentWave = 0;
}

// Called when the game starts or when spawned
void AASpawner::BeginPlay()
{
    Super::BeginPlay();

    // 트리거 이벤트 바인딩
    TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AASpawner::OnOverlapBegin);
}

void AASpawner::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
) 
{
    // 필요하면 여기서 플레이어인지 확인 (OtherActor == Player && !bHasTriggered)
    // 간단하게 GetPlayerCharacter 비교, 또는 캐릭터 캐스트 등
    // ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!bHasTriggered)
    {
        bHasTriggered = true;

        // 첫 웨이브 스폰 시작
        SpawnNextWave();
    }
};

void AASpawner::SpawnEnemy()
{
    // EnemyClass가 설정되지 않았다면 종료
    if (!EnemyClass) return;

    // SpawnActor (Deferred 방식 사용 예시)
    ACharacter* SpawnedEnemy = GetWorld()->SpawnActorDeferred<ACharacter>(
        EnemyClass,
        SpawnTransform,
        this,           // Owner
        nullptr,        // Instigator
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
    );

    if (SpawnedEnemy)
    {
        // 여기서 SpawnedEnemy에 대한 추가 초기화 가능
        // ex) 스폰 시 스포트라이트 꺼둠
        // SpawnedEnemy->bIsLightActive = false;

        // 스폰 마무리
        UGameplayStatics::FinishSpawningActor(SpawnedEnemy, SpawnTransform);
    }
}

void AASpawner::SpawnNextWave()
{
    // EnemyClass가 설정되지 않았다면 종료
    if (!EnemyClass) return;

    // 이번 웨이브에서 EnemiesPerWave 만큼 적 스폰
    for (int32 i = 0; i < EnemiesPerWave; ++i)
    {
        // 스폰
        ACharacter* SpawnedEnemy = GetWorld()->SpawnActor<ACharacter>(
            EnemyClass,
            SpawnTransform
        );

        if (SpawnedEnemy)
        {
            // Enemy 초기 설정(예: Hidden, AIController 등)
        }
    }

    // 웨이브 카운트 증가
    CurrentWave++;

    // 아직 남은 웨이브가 있으면 타이머로 다음 웨이브 호출
    if (CurrentWave < NumberOfWaves)
    {
        FTimerHandle TempHandle;  // 변수를 선언 (임시 객체 X)
        // WaveInterval초 후에 SpawnNextWave를 다시 호출
        GetWorldTimerManager().SetTimer(
            TempHandle,
            this,
            &AASpawner::SpawnNextWave,
            WaveInterval,
            false
        );
    }
}
