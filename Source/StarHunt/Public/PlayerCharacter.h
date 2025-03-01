#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UItemInventoryComponent;
class ABaseGun;
struct FInputActionValue;


UENUM(BlueprintType)
enum class ECurrentCharacterState : uint8
{
	None UMETA(DisplayName = "None"),
	Pistol UMETA(DisplayName = "Pistol"),
	Rifle UMETA(DisplayName = "Rifle"),
	Shotgun UMETA(DisplayName = "Shotgun")
};

UCLASS()
class STARHUNT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* TPSSpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* TPSZoomSpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* FPSSpringArm;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera", meta=(AllowPrivateAccess="true"))
	UChildActorComponent* TPSCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera", meta=(AllowPrivateAccess="true"))
	UChildActorComponent* TPSZoomCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera", meta=(AllowPrivateAccess="true"))
	UChildActorComponent* FPSCamera;
	//Inventory
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera", meta=(AllowPrivateAccess="true"))
	UItemInventoryComponent* ItemInventoryComponent;

	UPROPERTY(BlueprintReadWrite, Category = "Property")
	FRotator AimDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CurrentState")
	ECurrentCharacterState CurrentCharacterState;

	UFUNCTION(Blueprintable)
	void SetCurrentState(ECurrentCharacterState CharacterState);
	
protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	UFUNCTION()
	void StartCrouch(const FInputActionValue& value);
	UFUNCTION()
	void StopCrouch(const FInputActionValue& value);
	// UFUNCTION()
	// void StartCrouch(const FInputActionValue& value);
	// UFUNCTION()
	// void StopCrouch(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void CameraMode();
	UFUNCTION()
	void CameraDelayMode();
	UFUNCTION()
	void Zoom();
	UFUNCTION()
	void ResetZoom();

	//Inventory
	UFUNCTION()
	void ShowInventory();

private:
	float NormalSpeed;
	float SprintSpeedMultiplier;
	float SprintSpeed;
	float CameraBlendTime;

	bool IsTPSMode;
	bool IsZoomed;

	//Inventory
	bool bIsInventoryOpen;
	FTimerHandle CameraTransitionDelayHandle;

	ABaseGun* Weapon;
};
