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
