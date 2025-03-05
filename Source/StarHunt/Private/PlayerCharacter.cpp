#include "PlayerCharacter.h"

#include "BaseGun.h"
#include "EnhancedInputComponent.h"
#include "WraithPlayerController.h"
#include "ItemInventoryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemInventoryComponent = CreateDefaultSubobject<UItemInventoryComponent>(TEXT("Inventory"));

	NormalSpeed = 180.0f;
	SprintSpeedMultiplier = 3.0f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	CurrentWeapon = nullptr;

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
	PlayerAnimInstance = GetMesh()->GetAnimInstance();
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health : %f, DamageAmout : %f"), Health, DamageAmount);
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

			if (PlayerController->Fire)
			{
				EnhancedInput->BindAction(
					PlayerController->Fire,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::FireWeapon
				);

				EnhancedInput->BindAction(
					PlayerController->Fire,
					ETriggerEvent::Completed,
					this,
					&APlayerCharacter::StopFireWeapon
				);
			}

			if (PlayerController->Reload)
			{
				EnhancedInput->BindAction(
					PlayerController->Reload,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::ReloadWeapon
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
			if (PlayerController->QuickSlot1)
			{
				EnhancedInput->BindAction(
					PlayerController->QuickSlot1,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::UseQuickSlot0
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
		IsSprint = true;
	}
}

void APlayerCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
		IsSprint = false;
	}
}

void APlayerCharacter::StartCrouch(const FInputActionValue& value)
{
	if (GetCharacterMovement()->IsFalling()) return;

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
	SpawnWeapon(0);
}

void APlayerCharacter::Swap2()
{
	SpawnWeapon(1);
}

void APlayerCharacter::Swap3()
{
	SpawnWeapon(2);
}

void APlayerCharacter::SpawnWeapon(int32 EquipmentIndex)
{
	ABaseGun* WeaponInstance = ItemInventoryComponent->GetWeapon(EquipmentIndex);
	if (!WeaponInstance) return;

	UWorld* World = GetWorld();
	if (!World) return;

	TSubclassOf<ABaseGun> WeaponClass = WeaponInstance->GetClass();

	if (CurrentWeapon != nullptr && CurrentWeapon->GetClass() == WeaponInstance->GetClass()) // 여기가 문제인듯?
	{
		PlayAnimMontage(CurrentWeapon->UnEquipMontage);
		SetCurrentState(ECurrentCharacterState::None);
		return;
	}

	if (CurrentWeapon == nullptr)
	{
		CurrentWeapon = World->SpawnActor<ABaseGun>(WeaponClass);
	}
	else if (CurrentWeapon != nullptr && CurrentWeapon->GetClass() != WeaponInstance->GetClass())
	{
		PlayAnimMontage(CurrentWeapon->UnEquipMontage);
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
		CurrentWeapon = World->SpawnActor<ABaseGun>(WeaponClass);
	}

	SwapWeapon();
}

void APlayerCharacter::SwapWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *UEnum::GetValueAsString(CurrentWeapon->GunType));

	if (CurrentWeapon)
	{
		switch (CurrentWeapon->GunType)
		{
		case EGunType::Pistol:
			SetCurrentState(ECurrentCharacterState::Pistol);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			                                 "Pistol");
			break;
		case EGunType::Rifle:
			SetCurrentState(ECurrentCharacterState::Rifle);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			                                 "Rifle");
			break;
		case EGunType::ShotGun:
			SetCurrentState(ECurrentCharacterState::Shotgun);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			                                 "Shotgun");
			break;
		}

		PlayAnimMontage(CurrentWeapon->EquipMontage);
	}
}

void APlayerCharacter::FireWeapon()
{
	if (IsSprint) return;
	
	if (CurrentWeapon && !PlayerAnimInstance->Montage_IsPlaying(CurrentWeapon->FireMontage))
	{
		PlayAnimMontage(CurrentWeapon->FireMontage);
		// UE_LOG(LogTemp, Warning, TEXT("%s"), *CurrentWeapon->GetName());
		// CurrentWeapon->Fire();
	}
}

void APlayerCharacter::StopFireWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

void APlayerCharacter::ReloadWeapon()
{
	if (CurrentWeapon)
	{
		PlayAnimMontage(CurrentWeapon->ReloadMontage);
	}
}

void APlayerCharacter::UseQuickSlot0()
{
	if (ItemInventoryComponent)
	{
		ItemInventoryComponent->UseQuickSlotIem(0);
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
