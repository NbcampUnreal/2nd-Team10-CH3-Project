#include "DoorButton.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h" // 플레이어, 월드 기능
#include "Engine/World.h"
#include "GameFramework/Actor.h"

ADoorButton::ADoorButton()
{
	PrimaryActorTick.bCanEverTick = false;

    // 트리거 볼륨 생성
    TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
    RootComponent = TriggerVolume;

    // 버튼 메시 생성
    ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
    // 트리거 볼륨(루트)에 부착
    ButtonMesh->SetupAttachment(RootComponent);

    // 트리거 볼륨 충돌 세팅
    TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerVolume->SetCollisionObjectType(ECC_WorldStatic);
    TriggerVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
    // Pawn(플레이어)만 오버랩 받도록 설정
    TriggerVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ADoorButton::BeginPlay()
{
    Super::BeginPlay();

    // 트리거 볼륨의 OnComponentBeginOverlap 델리게이트에 함수 바인딩
    TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ADoorButton::OnOverlapBegin);
}

void ADoorButton::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{

    UE_LOG(LogTemp, Log, TEXT("문이 열렸다!"));
    // OtherActor가 플레이어 캐릭터인지, 또는 어떤 Pawn인지 검사 가능
    // ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    // if (OtherActor == PlayerChar) { ... }

    // DoorRefs에 들어 있는 모든 문을 열거나 HideDoor()를 호출
    for (AActor* DoorActor : DoorRefs)
    {
        if (DoorActor)
        {
            // 캐스팅 후 함수 호출 (예: HideDoor() 또는 OpenDoor())
            ADoor* MyDoor = Cast<ADoor>(DoorActor);
            if (MyDoor)
            {
                MyDoor->HideDoor();
                // 또는 MyDoor->OpenDoor();
            }
            else
            {
                // 문이 단순 AActor + Mesh라면
                UStaticMeshComponent* DoorMesh = Cast<UStaticMeshComponent>(
                    DoorActor->GetComponentByClass(UStaticMeshComponent::StaticClass())
                );
                if (DoorMesh)
                {
                    DoorMesh->SetVisibility(false);
                    DoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
                }
            }
        }
    }
}
