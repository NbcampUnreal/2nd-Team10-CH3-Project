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
	  Crouch(nullptr),
    InventoryOpenAction(nullptr),
    EquipmentOpenAction(nullptr)
{
}

void AWraithPlayerController::ShowInventory()
{
    if (InventoryHUDWidgetClass)
    {
        InventoryHUDWidgetInstance = CreateWidget<UUserWidget>(this, InventoryHUDWidgetClass);
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
void AWraithPlayerController::ShowDropItems(TArray<TSharedPtr<FString>*>* DropItems)
{
    OnDropItemsChange.Clear();
    if (DropItemsHUDWidgetClass && DropItems && DropItems->Num() > 0)
    {
        DropItemsHUDWidgetInstance = CreateWidget<UDropItemsWidget>(this, DropItemsHUDWidgetClass);
        if (DropItemsHUDWidgetInstance)
        {
            DropItemsHUDWidgetInstance->AddToViewport();
            DropItemsHUDWidgetInstance->InitInventory(DropItems);
        }
    }
}
void AWraithPlayerController::ShowDropItems(TArray<ADropItemActor*>* DropItemActors)
{
    if (DropItemsHUDWidgetClass && DropItemActors && DropItemActors->Num() > 0)
    {
        DropItemsHUDWidgetInstance = CreateWidget<UDropItemsWidget>(this, DropItemsHUDWidgetClass);
        if (DropItemsHUDWidgetInstance)
        {
            DropItemsHUDWidgetInstance->AddToViewport();
            DropItemsHUDWidgetInstance->InitInventory(DropItemActors);
        }
    }
}
void AWraithPlayerController::CloseDropItems()
{
    if (DropItemsHUDWidgetInstance)
    {
        DropItemsHUDWidgetInstance->RemoveFromParent();
        DropItemsHUDWidgetInstance = nullptr;
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
