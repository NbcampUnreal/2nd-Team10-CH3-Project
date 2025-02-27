#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
struct FInputActionValue;

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

	UPROPERTY(BlueprintReadWrite, Category = "Property")
	FRotator AimDirection;

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
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void CameraMode();
	UFUNCTION()
	void CameraDelayMode();
	UFUNCTION()
	void Zoom();
	UFUNCTION()
	void ResetZoom();

private:
	float NormalSpeed;
	float SprintSpeedMultiplier;
	float SprintSpeed;
	float CameraBlendTime;

	bool IsTPSMode;
	bool IsZoomed;
	FTimerHandle CameraTransitionDelayHandle;
};
