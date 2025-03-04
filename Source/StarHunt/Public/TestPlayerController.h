// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TestPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STARHUNT_API ATestPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

public:
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputMappingContext* InputMappingContext;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* MoveAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* LookAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* JumpAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* FireAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* ReloadAction;

    // 크로스헤어 위젯 클래스
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
    TSubclassOf<UUserWidget> HUDWidgetClass;

private:
    // 현재 활성화된 크로스헤어 위젯
    UPROPERTY()
    UUserWidget* CrosshairWidget;
};
