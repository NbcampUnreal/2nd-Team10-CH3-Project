#include "WraithPlayerController.h"

#include "EnhancedInputSubsystems.h"

AWraithPlayerController::AWraithPlayerController()
	: InputMappingContext(nullptr),
	  MoveAction(nullptr),
	  JumpAction(nullptr),
	  SprintAction(nullptr),
	  LookAction(nullptr),
	  CameraMode(nullptr),
	  Zoom(nullptr)
{
}

void AWraithPlayerController::ShowInventory()
{
    if (HUDWidgetClass)
    {
        HUDWidgetInstance = CreateWidget<UInventoryUserWidget>(this, HUDWidgetClass);
        if (HUDWidgetInstance)
        {
            HUDWidgetInstance->AddToViewport();
        }
    }
}

void AWraithPlayerController::CloseInventory()
{
    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
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

    if (HUDWidgetClass)
    {
        HUDWidgetInstance = CreateWidget<UInventoryUserWidget>(this, HUDWidgetClass);
        if (HUDWidgetInstance)
        {
            HUDWidgetInstance->AddToViewport();
        }
    }
}
