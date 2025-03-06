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
	
	UFUNCTION(Blueprintable)
	void SetCurrentState(ECurrentCharacterState CharacterState);
	UFUNCTION()
	float GetDamage() const;
	UFUNCTION()
	void SetDamage(const float Amount);
	
	UFUNCTION(BlueprintCallable)
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	UPROPERTY(BlueprintReadWrite, Category = "Property")
	FRotator AimDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CurrentState")
	ECurrentCharacterState CurrentCharacterState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	ABaseGun* CurrentWeapon;
	
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
	void SpawnWeapon(int32 EquipmentIndex);
	UFUNCTION()
	void SwapWeapon();
	UFUNCTION()
	void FireWeapon();
	UFUNCTION()
	void StopFireWeapon();
	UFUNCTION()
	void ReloadWeapon();
	UFUNCTION(BlueprintCallable)
	void PlayReloadAnim();
	UFUNCTION()
	void Recoil();
	UFUNCTION()
	void UseQuickSlot0();

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
	float Damage;
	float CameraBlendTime;
	bool IsSprint;
	FRotator CurrentRecoil;
	FRotator InitRecoil;
	
	UAnimInstance* PlayerAnimInstance;
	
	//Inventory
	bool bIsInventoryOpen;
	bool bIsEquipmentOpen;
	bool bIsDropItemsOpen;
	FTimerHandle CameraTransitionDelayHandle;

	ABaseGun* Weapon;
};

