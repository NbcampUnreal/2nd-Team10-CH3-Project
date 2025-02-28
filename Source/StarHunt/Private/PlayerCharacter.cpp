#include "PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "WraithPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	TPSSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("TPSSpringArm"));
	TPSSpringArm->SetupAttachment(RootComponent);
	TPSSpringArm->bUsePawnControlRotation = true;

	TPSZoomSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("TPSZoomSpringArm"));
	TPSZoomSpringArm->SetupAttachment(RootComponent);
	TPSZoomSpringArm->bUsePawnControlRotation = true;

	FPSSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("FPSSpringArm"));
	FPSSpringArm->SetupAttachment(GetMesh(), TEXT("head"));
	FPSSpringArm->bUsePawnControlRotation = true;

	TPSCamera = CreateDefaultSubobject<UChildActorComponent>(TEXT("TPSCamera"));
	TPSCamera->SetupAttachment(TPSSpringArm);

	TPSZoomCamera = CreateDefaultSubobject<UChildActorComponent>(TEXT("TPSZoomCamera"));
	TPSZoomCamera->SetupAttachment(TPSZoomSpringArm);

	FPSCamera = CreateDefaultSubobject<UChildActorComponent>(TEXT("FPSCamera"));
	FPSCamera->SetupAttachment(FPSSpringArm);

	//Inventory
	ItemInventoryComponent = CreateDefaultSubobject<UItemInventoryComponent>(TEXT("Inventory"));

	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.7f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	IsTPSMode = true;
	bIsInventoryOpen = false;
}

void APlayerCharacter::SetCurrentState(ECurrentCharacterState CharacterState)
{
	CurrentCharacterState = CharacterState;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AWraithPlayerController* PlayerController = Cast<AWraithPlayerController>(GetController()))
	{
		PlayerController->SetViewTargetWithBlend(TPSCamera->GetChildActor(), 0, VTBlend_Linear, 0, false);
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AWraithPlayerController* PlayerController = Cast<AWraithPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::Move
				);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::StartJump
				);
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&APlayerCharacter::StopJump
				);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::StartSprint
				);
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&APlayerCharacter::StopSprint
				);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::Look
				);
			}

			if (PlayerController->CameraMode)
			{
				EnhancedInput->BindAction(
					PlayerController->CameraMode,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::CameraDelayMode
				);
			}

			if (PlayerController->Zoom)
			{
				EnhancedInput->BindAction(
					PlayerController->Zoom,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::Zoom
				);

				EnhancedInput->BindAction(
					PlayerController->Zoom,
					ETriggerEvent::Completed,
					this,
					&APlayerCharacter::ResetZoom
				);

				EnhancedInput->BindAction(
					PlayerController->Zoom,
					ETriggerEvent::Canceled,
					this,
					&APlayerCharacter::ResetZoom
				);
			}

			if (PlayerController->Crouch)
			{
				EnhancedInput->BindAction(
					PlayerController->Crouch,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::StartCrouch
				);

				EnhancedInput->BindAction(
					PlayerController->Crouch,
					ETriggerEvent::Completed,
					this,
					&APlayerCharacter::StopCrouch
				);
			}
		

			if (PlayerController->InventoryOpenAction)
			{
				EnhancedInput->BindAction(
					PlayerController->InventoryOpenAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::ShowInventory
				);
			}
		}
	}
}

void APlayerCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void APlayerCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
	if (ItemInventoryComponent)
	{
		ItemInventoryComponent->GetWeapon(0);
	}
}

void APlayerCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

void APlayerCharacter::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void APlayerCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

void APlayerCharacter::StartCrouch(const FInputActionValue& value)
{
	Crouch();
	UE_LOG(LogTemp, Warning, TEXT("Crouch!!!!!!!!!!!!!!"));
}

void APlayerCharacter::StopCrouch(const FInputActionValue& value)
{
	UnCrouch();
	UE_LOG(LogTemp, Warning, TEXT("StopCrouch!!!!!!!!!!!!!!"));
}

// void APlayerCharacter::StartCrouch(const FInputActionValue& value)
// {
// }
//
// void APlayerCharacter::StopCrouch(const FInputActionValue& value)
// {
// }

void APlayerCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);

	auto Delta = GetControlRotation() - GetActorRotation();
	Delta.Normalize();

	AimDirection = FMath::RInterpTo(AimDirection, Delta, GetWorld()->GetDeltaSeconds(), 45.0f);
	AimDirection = FRotator(
		FMath::ClampAngle(AimDirection.Pitch, -90, 90),
		FMath::ClampAngle(AimDirection.Yaw, -90, 90),
		0);
}

void APlayerCharacter::CameraMode()
{
	if (IsZoomed) return;

	AWraithPlayerController* PlayerController = Cast<AWraithPlayerController>(GetController());
	if (!PlayerController) return;

	if (IsTPSMode)
	{
		PlayerController->SetViewTargetWithBlend(FPSCamera->GetChildActor(), 0, VTBlend_Linear, 0, false);
	}
	else
	{
		PlayerController->SetViewTargetWithBlend(TPSCamera->GetChildActor(), 0, VTBlend_Linear, 0, false);
	}

	IsTPSMode = !IsTPSMode;
}

void APlayerCharacter::CameraDelayMode()
{
	GetWorld()->GetTimerManager().SetTimer(
		CameraTransitionDelayHandle,
		this,
		&APlayerCharacter::CameraMode,
		0.2f,
		false
	);
}

void APlayerCharacter::Zoom()
{
	if (!IsTPSMode || IsZoomed) return;

	AWraithPlayerController* PlayerController = Cast<AWraithPlayerController>(GetController());
	if (!PlayerController) return;

	PlayerController->SetViewTargetWithBlend(TPSZoomCamera->GetChildActor(), 0, VTBlend_Linear, 0, false);
	IsZoomed = true;
}

void APlayerCharacter::ResetZoom()
{
	if (!IsZoomed) return;

	AWraithPlayerController* PlayerController = Cast<AWraithPlayerController>(GetController());
	if (!PlayerController) return;

	PlayerController->SetViewTargetWithBlend(TPSCamera->GetChildActor(), 0, VTBlend_Linear, 0, false);

	IsZoomed = false;
}

void APlayerCharacter::ShowInventory()
{
	if (AWraithPlayerController* PlayerController = Cast<AWraithPlayerController>(GetController()))
	{
		if (bIsInventoryOpen)
		{
			PlayerController->CloseInventory();
			PlayerController->bShowMouseCursor = false;
			bIsInventoryOpen = false;
		}
		else
		{
			PlayerController->ShowInventory();
			PlayerController->bShowMouseCursor = true;
			bIsInventoryOpen = true;
		}
	}
}
