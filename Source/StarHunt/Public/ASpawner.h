#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include "ASpawner.generated.h"

// 전방 선언
class AEnemyWithSpotLight;

UCLASS()
class STARHUNT_API AASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AASpawner();

	// 트리거 볼륨
	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	UBoxComponent* TriggerVolume;

	// 적 클래스 (BP_EnemyWithSpotLight)
	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<ACharacter> EnemyClass;

	// 스폰 위치/회전
	UPROPERTY(EditAnywhere, Category = "Spawn")
	FTransform SpawnTransform;

	// 웨이브 관련 설정
	UPROPERTY(EditAnywhere, Category = "Spawning")
	int32 NumberOfWaves = 3;      // 총 웨이브 수
	UPROPERTY(EditAnywhere, Category = "Spawning")
	int32 EnemiesPerWave = 2;     // 웨이브당 스폰할 적 수
	UPROPERTY(EditAnywhere, Category = "Spawning")
	float WaveInterval = 5.f;     // 웨이브 간 간격(초)

	// 트리거 이벤트
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bHasTriggered;    // 한 번만 트리거 처리할지 여부
	int32 CurrentWave;     // 현재 웨이브 인덱스 (0 ~ NumberOfWaves-1)

	// 적 스폰용 함수
	void SpawnEnemy();

	// 웨이브 스폰 함수
	void SpawnNextWave();

};
