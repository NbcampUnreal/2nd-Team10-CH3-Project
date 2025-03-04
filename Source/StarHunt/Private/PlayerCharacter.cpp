#include "PlayerCharacter.h"

#include "BaseGun.h"
#include "EnhancedInputComponent.h"
#include "WraithPlayerController.h"
#include "ItemInventoryComponent.h"
#include "BaseGun.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemInventoryComponent = CreateDefaultSubobject<UItemInventoryComponent>(TEXT("Inventory"));

	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.7f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	bIsInventoryOpen = false;
	bIsEquipmentOpen = false;
	bIsDropItemsOpen = false;

	Health=MaxHealth=100;
}

void APlayerCharacter::SetCurrentState(ECurrentCharacterState CharacterState)
{
	CurrentCharacterState = CharacterState;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	if (Health <= 0.0f) 
	{
		//Death
	}
	return ActualDamage;
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
					&APlayerCharacter::CameraMode
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

			if (PlayerController->Swap1)
			{
				EnhancedInput->BindAction(
					PlayerController->Swap1,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::Swap1
				);
			}

			if (PlayerController->Swap2)
			{
				EnhancedInput->BindAction(
					PlayerController->Swap2,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::Swap2
				);
			}

			if (PlayerController->Swap3)
			{
				EnhancedInput->BindAction(
					PlayerController->Swap3,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::Swap3
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
			if (PlayerController->EquipmentOpenAction)
			{
				EnhancedInput->BindAction(
					PlayerController->EquipmentOpenAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::ShowEquipment
				);
			}
			if (PlayerController->DropItemsOpenAction)
			{
				EnhancedInput->BindAction(
					PlayerController->DropItemsOpenAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::ShowDropItems
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
}

void APlayerCharacter::StopCrouch(const FInputActionValue& value)
{
	UnCrouch();
}

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

void APlayerCharacter::Swap1()
{
	SwapWeapon(0);
}

void APlayerCharacter::Swap2()
{
	SwapWeapon(1);
}

void APlayerCharacter::Swap3()
{
	SwapWeapon(2);
}

void APlayerCharacter::SwapWeapon(int32 EquipmentIndex)
{
	ABaseGun* WeaponInstance = Cast<ABaseGun>(ItemInventoryComponent->GetWeapon(EquipmentIndex));
	if (!WeaponInstance) return;
	
	UWorld* World = GetWorld();
	if (!World) return;

	TSubclassOf<ABaseGun> WeaponClass = WeaponInstance->GetClass();

	if (CurrentWeapon == WeaponInstance)
	{
		PlayAnimMontage(CurrentWeapon->UnEquipMontage);
	}
	else if (CurrentWeapon == nullptr || CurrentWeapon != WeaponInstance)
	{
		CurrentWeapon = World->SpawnActor<ABaseGun>(WeaponClass);
	}

	if (CurrentWeapon)
	{
		switch (CurrentWeapon->GunType)
		{
		case EGunType::Pistol:
			SetCurrentState(ECurrentCharacterState::Pistol);
			AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Pistol");
			break;
		case EGunType::Rifle:
			SetCurrentState(ECurrentCharacterState::Rifle);
			AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Rifle");
			break;
		case EGunType::ShotGun:
			SetCurrentState(ECurrentCharacterState::Shotgun);
			AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Shotgun");
			break;
		}

		PlayAnimMontage(CurrentWeapon->EquipMontage);
	}
}

void APlayerCharacter::DestroyCurrentWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}
}

void APlayerCharacter::ShowInventory()
{
	if (AWraithPlayerController* PlayerController = Cast<AWraithPlayerController>(GetController()))
	{
		if (bIsInventoryOpen)
		{
			PlayerController->CloseInventory();
			bIsInventoryOpen = false;
			if (!bIsOpenWindows())
			{
				PlayerController->bShowMouseCursor = false;
			}
		}
		else
		{
			PlayerController->ShowInventory();
			PlayerController->bShowMouseCursor = true;
			bIsInventoryOpen = true;
		}
	}
}

void APlayerCharacter::ShowEquipment()
{
	if (AWraithPlayerController* PlayerController = Cast<AWraithPlayerController>(GetController()))
	{
		if (bIsEquipmentOpen)
		{
			PlayerController->CloseEquipment();
			bIsEquipmentOpen = false;
			if (!bIsOpenWindows())
			{
				PlayerController->bShowMouseCursor = false;
			}
		}
		else
		{
			PlayerController->ShowEquipment();
			PlayerController->bShowMouseCursor = true;
			bIsEquipmentOpen = true;
		}
	}
}

void APlayerCharacter::ShowDropItems()
{
	if (AWraithPlayerController* PlayerController = Cast<AWraithPlayerController>(GetController()))
	{
		if (bIsDropItemsOpen)
		{
			PlayerController->CloseDropItems();
			bIsDropItemsOpen = false;
			if (!bIsOpenWindows())
			{
				PlayerController->bShowMouseCursor = false;
			}
		}
		else
		{
			if (ItemInventoryComponent)
			{
				ItemInventoryComponent->GetNearbyItemActors();
				PlayerController->ShowDropItems(&ItemInventoryComponent->OverlappingItemActors);
				PlayerController->bShowMouseCursor = true;
			}
			bIsDropItemsOpen = true;
		}
	}
}

bool APlayerCharacter::bIsOpenWindows()
{
	if (bIsInventoryOpen || bIsEquipmentOpen || bIsDropItemsOpen)
	{
		return true;
	}
	return false;
}
