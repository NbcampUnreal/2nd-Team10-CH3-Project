// Fill out Mer copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Me.generated.h"

UCLASS()
class STARHUNT_API AMe : public ACharacter
{
	GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AMe();

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
    UStaticMeshComponent* CubeMesh;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
    class USpringArmComponent* SpringArm;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
    class UCameraComponent* Camera;

    // 예: 플레이어가 사용하는 스켈레탈 메시 컴포넌트가 GetMesh()라 가정
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Weapon")
    USceneComponent* WeaponSocketComponent;

    // 마우스 감도 조절 변수
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Input")
    float SocketPitchSensitivity;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Weapon")
    TSubclassOf<class ABaseGun> GunClass;

    ABaseGun* EquippedGun;

    void Move(const struct FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void StartJump(const FInputActionValue& Value);
    void StopJump(const FInputActionValue& Value);
    void Fire(const FInputActionValue& Value);
    void StopFire(const FInputActionValue& Value);
    void Reload(const FInputActionValue& Value);

};
