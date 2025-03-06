#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ItemShowWidget.h"
#include "WraithPlayerController.generated.h"


class UInputMappingContext;
class UInputAction;

DECLARE_MULTICAST_DELEGATE(FOnDropItemsChange);

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Input")
	UInputAction* Fire;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Input")
	UInputAction* Reload;
	
	//Inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* InventoryOpenAction;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* EquipmentOpenAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* DropItemsOpenAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* QuickSlot1;
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ShowInventory();
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void CloseInventory();
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ShowEquipment();
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void CloseEquipment();

	void ShowDropItems(TArray<TSharedPtr<FString>>* DropItems);
	void ShowDropItems(TArray<ADropItemActor*>* DropItemActors);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void CloseDropItems();

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowGameOver();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UItemShowWidget> ItemShowHUDWidgetClass;
	UItemShowWidget* ItemShowHUDWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameOverHUDWidgetClass;
	UUserWidget* GameOverHUDWidgetInstance;

	FOnDropItemsChange OnDropItemsChange;
protected:
	virtual void BeginPlay() override;
};
