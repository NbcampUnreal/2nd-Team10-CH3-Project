#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InventoryUserWidget.h"
#include "WraithPlayerController.generated.h"


class UInputMappingContext;
class UInputAction;

UCLASS()
class STARHUNT_API AWraithPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AWraithPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Input")
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Input")
	UInputAction* CameraMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Input")
	UInputAction* Zoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Input")
	UInputAction* Crouch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Input")
	UInputAction* Swap1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Input")
	UInputAction* Swap2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Input")
	UInputAction* Swap3;
	//Inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* InventoryOpenAction;

	//Inventory UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UInventoryUserWidget> HUDWidgetClass;
	UInventoryUserWidget* HUDWidgetInstance;
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ShowInventory();
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void CloseInventory();
protected:
	virtual void BeginPlay() override;
};
