// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TestPlayer.generated.h"

UCLASS()
class STARHUNT_API ATestPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATestPlayer();

public:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USceneComponent* Scene;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* CubeMesh;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USpringArmComponent* SpringArm;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UCameraComponent* Camera;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void Move(const struct FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void StartJump(const FInputActionValue& Value);
    void StopJump(const FInputActionValue& Value);
    void Fire(const FInputActionValue& Value);

protected:

    /** 점프 여부 */
    bool bIsJumping;

    // 이동에 사용할 힘의 크기
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MovementForce;

    /** 점프 높이 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float JumpImpulse;

    /** 중력 값 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float Gravity;

    /** 착지 감지 */
    UFUNCTION()
    void OnLanded(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
