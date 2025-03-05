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
	  Swap1(nullptr),
	  Swap2(nullptr),
	  Swap3(nullptr),
	  Fire(nullptr),
	  Reload(nullptr)
    QuickSlot1(nullptr),
    InventoryOpenAction(nullptr),
    EquipmentOpenAction(nullptr)
{
}

void AWraithPlayerController::ShowInventory()
{
    if (ItemShowHUDWidgetClass)
    {
        if (!ItemShowHUDWidgetInstance)
        {
            ItemShowHUDWidgetInstance = CreateWidget<UItemShowWidget>(this, ItemShowHUDWidgetClass);

        }
        if (ItemShowHUDWidgetInstance)
        {
            ItemShowHUDWidgetInstance->AddToViewport();
            ItemShowHUDWidgetInstance->ShowInventory();
        }
    }
}
void AWraithPlayerController::CloseInventory()
{
    if (ItemShowHUDWidgetInstance)
    {
        ItemShowHUDWidgetInstance->CloseInventory();
    }
}

void AWraithPlayerController::ShowEquipment()
{
    if (ItemShowHUDWidgetClass)
    {
        if (!ItemShowHUDWidgetInstance)
        {
            ItemShowHUDWidgetInstance = CreateWidget<UItemShowWidget>(this, ItemShowHUDWidgetClass);

        }
        if (ItemShowHUDWidgetInstance)
        {
            ItemShowHUDWidgetInstance->AddToViewport();
            ItemShowHUDWidgetInstance->ShowEquipment();
        }
    }
}

void AWraithPlayerController::CloseEquipment()
{
    if (ItemShowHUDWidgetInstance)
    {
        ItemShowHUDWidgetInstance->CloseEquipment();
    }
}
void AWraithPlayerController::ShowDropItems(TArray<ADropItemActor*>* DropItemActors)
{
    if (ItemShowHUDWidgetClass)
    {
        if (!ItemShowHUDWidgetInstance)
        {
            ItemShowHUDWidgetInstance = CreateWidget<UItemShowWidget>(this, ItemShowHUDWidgetClass);

        }
        if (ItemShowHUDWidgetInstance)
        {
            ItemShowHUDWidgetInstance->AddToViewport();
            ItemShowHUDWidgetInstance->ShowDropItems(DropItemActors);
        }
    }
}

void AWraithPlayerController::CloseDropItems()
{
    if (ItemShowHUDWidgetInstance)
    {
        ItemShowHUDWidgetInstance->CloseDropItems();
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
