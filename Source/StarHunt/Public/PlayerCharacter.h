#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class ABaseGun;
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

	//Inventory
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera", meta=(AllowPrivateAccess="true"))
	UItemInventoryComponent* ItemInventoryComponent;

	UPROPERTY(BlueprintReadWrite, Category = "Property")
	FRotator AimDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CurrentState")
	ECurrentCharacterState CurrentCharacterState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	ABaseGun* CurrentWeapon;
	
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
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION(BlueprintImplementableEvent, Category= "Input")
	void CameraMode();
	UFUNCTION(BlueprintImplementableEvent, Category= "Input")
	void Zoom();
	UFUNCTION(BlueprintImplementableEvent, Category= "Input")
	void ResetZoom();
	UFUNCTION()
	void Swap1();
	UFUNCTION()
	void Swap2();
	UFUNCTION()
	void Swap3();
	UFUNCTION()
	void SwapWeapon(int32 EquipmentIndex);

	UFUNCTION(BlueprintCallable)
	void DestroyCurrentWeapon();
	
	//Inventory
	UFUNCTION()
	void ShowInventory();

	UFUNCTION()
	void ShowEquipment();

	UFUNCTION()
	void ShowDropItems();

	UFUNCTION()
	bool bIsOpenWindows();
private:
	float NormalSpeed;
	float SprintSpeedMultiplier;
	float SprintSpeed;
	float CameraBlendTime;

	bool IsSpawnedWeapon;

	//Inventory
	bool bIsInventoryOpen;
	bool bIsEquipmentOpen;
	bool bIsDropItemsOpen;
	FTimerHandle CameraTransitionDelayHandle;

	ABaseGun* Weapon;
};
