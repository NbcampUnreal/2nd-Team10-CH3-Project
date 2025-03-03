#include "WraithPlayerController.h"

#include "EnhancedInputSubsystems.h"

AWraithPlayerController::AWraithPlayerController()
	: InputMappingContext(nullptr),
	  MoveAction(nullptr),
	  JumpAction(nullptr),
	  SprintAction(nullptr),
	  LookAction(nullptr),
	  CameraMode(nullptr),
	  Zoom(nullptr),
	  Crouch(nullptr)
{
}

void AWraithPlayerController::ShowInventory()
{
    if (InventoryHUDWidgetClass)
    {
        InventoryHUDWidgetInstance = CreateWidget<UInventoryUserWidget>(this, InventoryHUDWidgetClass);
        if (InventoryHUDWidgetInstance)
        {
            InventoryHUDWidgetInstance->AddToViewport();
        }
    }
}

void AWraithPlayerController::CloseInventory()
{
    if (InventoryHUDWidgetInstance)
    {
        InventoryHUDWidgetInstance->RemoveFromParent();
        InventoryHUDWidgetInstance = nullptr;
    }
}

void AWraithPlayerController::ShowEquipment()
{
    if (EquipmentHUDWidgetClass)
    {
        EquipmentHUDWidgetInstance = CreateWidget<UUserWidget>(this, EquipmentHUDWidgetClass);
        if (EquipmentHUDWidgetInstance)
        {
            EquipmentHUDWidgetInstance->AddToViewport();
        }
    }
}

void AWraithPlayerController::CloseEquipment()
{
    if (EquipmentHUDWidgetInstance)
    {
        EquipmentHUDWidgetInstance->RemoveFromParent();
        EquipmentHUDWidgetInstance = nullptr;
    }
}
void AWraithPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    	{
    		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = LocalPlayer->GetSubsystem<
    			UEnhancedInputLocalPlayerSubsystem>())
    		{
    			if (InputMappingContext)
    			{
    				SubSystem->AddMappingContext(InputMappingContext, 0);
    			}
    		}
    	}

}
