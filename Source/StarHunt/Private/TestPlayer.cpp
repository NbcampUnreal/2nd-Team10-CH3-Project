// Fill out Mer copyright notice in the Description page of Project Settings.


#include "TestPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "TestPlayerController.h"

ATestPlayer::ATestPlayer()
{
    PrimaryActorTick.bCanEverTick = false;

    // 새로운 루트 컴포넌트로 Scene 설정
    /*Scene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    SetRootComponent(Scene);*/
    // 정육면체 메시 추가 및 Scene에 부착
    CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
    SetRootComponent(CubeMesh);
        //CubeMesh->SetupAttachment(Scene);

    // 스프링암 추가 및 Scene에 부착
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(CubeMesh);
    SpringArm->TargetArmLength = 300.0f;
    SpringArm->bUsePawnControlRotation = true; // 플레이어가 마우스로 카메라 회전 가능

    // 카메라 추가 및 스프링암에 부착
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
    Camera->bUsePawnControlRotation = false; // 카메라는 별도로 회전하지 않음

    // 점프 기본값 설정
    bIsJumping = false;
    JumpImpulse = 600.0f;
    Gravity = 980.0f; // 중력값 설정 (UE 기본 중력 980)
    MovementForce = 500000.0f; // 필요에 따라 값을 조정

    // 물리 적용 가능하도록 설정
    CubeMesh->SetSimulatePhysics(true);
    CubeMesh->SetEnableGravity(true);
    CubeMesh->SetCollisionProfileName(TEXT("PhysicsActor"));

    // 착지 감지를 위한 델리게이트 연결
    CubeMesh->OnComponentHit.AddDynamic(this, &ATestPlayer::OnLanded);
}

void ATestPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Enhanced InputComponent로 캐스팅
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // IA를 가져오기 위해 현재 소유 중인 Controller를 ASpartaPlayerController로 캐스팅
        if (ATestPlayerController* PlayerController = Cast<ATestPlayerController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                // IA_Move 액션 키를 "키를 누르고 있는 동안" Move() 호출
                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ATestPlayer::Move
                );
            }

            if (PlayerController->JumpAction)
            {
                // IA_Jump 액션 키를 "키를 누르고 있는 동안" StartJump() 호출
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ATestPlayer::StartJump
                );

                // IA_Jump 액션 키에서 "손을 뗀 순간" StopJump() 호출
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Completed,
                    this,
                    &ATestPlayer::StopJump
                );
            }

            if (PlayerController->LookAction)
            {
                // IA_Look 액션 마우스가 "움직일 때" Look() 호출
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ATestPlayer::Look
                );
            }

            if (PlayerController->FireAction)
            {
                // IA_Look 액션 마우스가 "움직일 때" Look() 호출
                EnhancedInput->BindAction(
                    PlayerController->FireAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ATestPlayer::Fire
                );
            }
        }
    }
}


void ATestPlayer::Move(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Warning, TEXT("move"));

    if (!Controller) return;

    // Value는 Axis2D로 설정된 IA_Move의 입력값 (WASD)을 담고 있음
// 예) (X=1, Y=0) → 전진 / (X=-1, Y=0) → 후진 / (X=0, Y=1) → 오른쪽 / (X=0, Y=-1) → 왼쪽
    const FVector2D MoveInput = Value.Get<FVector2D>();

    // Pawn의 방향 기준으로 힘을 계산하여 CubeMesh에 적용
    FVector ForceToAdd = (GetActorForwardVector() * MoveInput.X + GetActorRightVector() * MoveInput.Y) * MovementForce;
    CubeMesh->AddForce(ForceToAdd);
}

void ATestPlayer::Look(const FInputActionValue& Value)
{
    FVector2D LookVector = Value.Get<FVector2D>();

    AddControllerYawInput(LookVector.X);
    AddControllerPitchInput(LookVector.Y);
}

void ATestPlayer::StartJump(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Warning, TEXT("jump"));

    if (!bIsJumping)
    {
        bIsJumping = true;

        // Y축 방향으로 힘 추가 (위쪽으로 점프)
        FVector JumpForce = FVector(0, 0, JumpImpulse);
        CubeMesh->AddImpulse(JumpForce, NAME_None, true);
    }
}

void ATestPlayer::StopJump(const FInputActionValue& Value)
{
    if (bIsJumping)
    {
        bIsJumping = false;

        // 점프 중지 시, 중력 복원
        FVector GravityForce = FVector(0, 0, -Gravity * CubeMesh->GetMass());
        CubeMesh->AddForce(GravityForce);
    }
}

void ATestPlayer::Fire(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Warning, TEXT("fire"));

}

void ATestPlayer::OnLanded(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    bIsJumping = false;
}
