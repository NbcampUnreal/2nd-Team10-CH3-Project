// Fill out Mer copyright notice in the Description page of Project Settings.


#include "Me.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "TestPlayerController.h"
#include "BaseGun.h"

AMe::AMe()
{
    PrimaryActorTick.bCanEverTick = false;

    // 정육면체 메시 추가 및 Scene에 부착
    CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
    CubeMesh->SetupAttachment(RootComponent);

    // 스프링암 추가 및 Scene에 부착
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(CubeMesh);
    SpringArm->TargetArmLength = 300.0f;
    SpringArm->bUsePawnControlRotation = true; // 플레이어가 마우스로 카메라 회전 가능

    // 카메라 추가 및 스프링암에 부착
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
    Camera->bUsePawnControlRotation = false; // 카메라는 별도로 회전하지 않음

    // 예: "WeaponSocket"이라는 소켓에 부착
    WeaponSocketComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponSocketComponent"));
    if (GetMesh())
    {
        WeaponSocketComponent->SetupAttachment(CubeMesh, FName("GunSocket"));
    }
    // 감도 초기값 (원하는 값으로 조정)
    SocketPitchSensitivity = 1.0f;
}

void AMe::BeginPlay()
{
    Super::BeginPlay();

    if (GunClass)
    {
        EquippedGun = GetWorld()->SpawnActor<ABaseGun>(GunClass);
        if (EquippedGun)
        {
            EquippedGun->AttachToComponent(WeaponSocketComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
            EquippedGun->SetOwner(this);
        }
    }
}

void AMe::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (ATestPlayerController* PlayerController = Cast<ATestPlayerController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AMe::Move
                );
            }

            if (PlayerController->JumpAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AMe::StartJump
                );

                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Completed,
                    this,
                    &AMe::StopJump
                );
            }

            if (PlayerController->LookAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AMe::Look
                );
            }

            if (PlayerController->FireAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->FireAction,
                    ETriggerEvent::Started,
                    this,
                    &AMe::Fire
                );
            }
            if (PlayerController->FireAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->FireAction,
                    ETriggerEvent::Completed,
                    this,
                    &AMe::StopFire
                );
            }

            if (PlayerController->ReloadAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->ReloadAction,
                    ETriggerEvent::Started,
                    this,
                    &AMe::Reload
                );
            }
        }
    }
}


void AMe::Move(const FInputActionValue& Value)
{
    if (!Controller) return;

    const FVector2D MoveInput = Value.Get<FVector2D>();

    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        AddMovementInput(GetActorForwardVector(), MoveInput.X);
    }

    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        AddMovementInput(GetActorRightVector(), MoveInput.Y);
    }
}

void AMe::Look(const FInputActionValue& Value)
{
    FVector2D LookVector = Value.Get<FVector2D>();

    AddControllerYawInput(LookVector.X);
    AddControllerPitchInput(LookVector.Y);
}

void AMe::StartJump(const FInputActionValue& Value)
{
    if (Value.Get<bool>())
    {
        Jump();
    }
}

void AMe::StopJump(const FInputActionValue& Value)
{
    if (!Value.Get<bool>())
    {
        StopJumping();
    }
}

void AMe::Fire(const FInputActionValue& Value)
{
    if (EquippedGun)
    {
        EquippedGun->Fire();
    }
}

void AMe::StopFire(const FInputActionValue& Value)
{
    if (EquippedGun)
    {
        EquippedGun->StopFire();
    }
}

void AMe::Reload(const FInputActionValue& Value)
{
    if (EquippedGun)
    {
        EquippedGun->Reload();
    }
}
