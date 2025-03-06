#include "Door.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = false;

	// 문 메시 컴포넌트 생성
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	RootComponent = DoorMesh;

	// 초기 충돌, 모빌리티 설정
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DoorMesh->SetMobility(EComponentMobility::Movable); // 문을 회전/이동 가능하게
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADoor::HideDoor()
{
    // 메시 숨기기
    DoorMesh->SetVisibility(false, true);
    // 충돌 비활성화
    DoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADoor::ShowDoor()
{
    // 메시 보이기
    DoorMesh->SetVisibility(true, true);
    // 충돌 활성화
    DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}