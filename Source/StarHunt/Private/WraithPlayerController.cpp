#include "WraithPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "ShooterGameInstance.h"
#include "Kismet/GameplayStatics.h"

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
	  Reload(nullptr),
    QuickSlot1(nullptr),
    InventoryOpenAction(nullptr),
    EquipmentOpenAction(nullptr),
    MainMenuClass(nullptr),
    MainMenuWidgetInstance(nullptr)
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

void AWraithPlayerController::ShowGameOver()
{
    if (ItemShowHUDWidgetInstance)
    {
        ItemShowHUDWidgetInstance->RemoveFromParent();
        ItemShowHUDWidgetInstance = nullptr;
    }

    if (GameOverHUDWidgetClass)
    {
        GameOverHUDWidgetInstance = CreateWidget<UUserWidget>(this, GameOverHUDWidgetClass);
        if (GameOverHUDWidgetInstance)
        {
            GameOverHUDWidgetInstance->AddToViewport();

            bShowMouseCursor = true;
            SetInputMode(FInputModeUIOnly());
        }
    }
}

void AWraithPlayerController::ShowGameClear()
{
    if (ItemShowHUDWidgetInstance)
    {
        ItemShowHUDWidgetInstance->RemoveFromParent();
        ItemShowHUDWidgetInstance = nullptr;
    }

    if (GameClearHUDWidgetClass)
    {
        GameClearHUDWidgetInstance = CreateWidget<UUserWidget>(this, GameClearHUDWidgetClass);
        if (GameClearHUDWidgetInstance)
        {
            GameClearHUDWidgetInstance->AddToViewport();

            bShowMouseCursor = true;
            SetInputMode(FInputModeUIOnly());
        }
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
    //게임 실행 ㅡ> 메인 레벨에서 메인 메뉴 UI 표시
    FString CurrentMapName=GetWorld()->GetMapName();
    if (CurrentMapName.Contains("MainMenu"))
    {
        ShowMainMenu();
    }
}

void AWraithPlayerController::ShowMainMenu()
{
    //이미 켜진 메뉴들 다 끄기
    if (ItemShowHUDWidgetInstance)
    {
        ItemShowHUDWidgetInstance->RemoveFromParent();
        ItemShowHUDWidgetInstance = nullptr;
    }
    if (GameClearHUDWidgetInstance)
    {
        GameClearHUDWidgetInstance->RemoveFromParent();
        GameClearHUDWidgetInstance = nullptr;
    }
    if (GameOverHUDWidgetInstance)
    {
        GameOverHUDWidgetInstance->RemoveFromParent();
        GameOverHUDWidgetInstance = nullptr;
    }
    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }
    //메뉴 UI 생성
    if (MainMenuClass)
    {
        MainMenuWidgetInstance=CreateWidget<UUserWidget>(this,MainMenuClass);
        if (MainMenuWidgetInstance)
        {
            MainMenuWidgetInstance->AddToViewport();
            bShowMouseCursor = true;
            SetInputMode(FInputModeUIOnly());
        }
    }
}

void AWraithPlayerController::StartGame()
{
    if (MainMenuWidgetInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("Called"));
        MainMenuWidgetInstance->RemoveFromParent();
        bShowMouseCursor = false;
        SetInputMode(FInputModeGameOnly());
    }
    if (UShooterGameInstance* GameInstance =Cast<UShooterGameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        GameInstance->TotalScore=0;
        GameInstance->TotalPlayTimeMinute=0;
        GameInstance->TotalPlayTimeSec=0;
        GameInstance->CurrentWave=0;
    }
    UGameplayStatics::OpenLevel(GetWorld(), FName("Map_SpaceShip"));
    
}
