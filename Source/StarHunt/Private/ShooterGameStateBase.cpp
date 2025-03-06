// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "WraithPlayerController.h"
#include "ItemBlueprintFunctionLibrary.h"
#include "ShooterGameInstance.h"

AShooterGameStateBase::AShooterGameStateBase()
{
	StartTime = 0.0f;
}

void AShooterGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		StartTime = GetWorld()->GetTimeSeconds();
	}
}

void AShooterGameStateBase::RestartLevel()
{
	if (GetWorld())
	{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			if (AWraithPlayerController* WraithPlayerController = Cast<AWraithPlayerController>(PlayerController))
			{
				WraithPlayerController->SetPause(false);
				WraithPlayerController->bShowMouseCursor = false;
				WraithPlayerController->SetInputMode(FInputModeGameOnly());
			}
		}

		UGameplayStatics::OpenLevel(GetWorld(), *GetWorld()->GetMapName());
	}
}

void AShooterGameStateBase::ResetGame()
{
	if (GetWorld())
	{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			if (AWraithPlayerController* WraithPlayerController = Cast<AWraithPlayerController>(PlayerController))
			{
				WraithPlayerController->SetPause(false);
				WraithPlayerController->bShowMouseCursor = false;
				WraithPlayerController->SetInputMode(FInputModeGameOnly());
			}
		}
		if (UItemSubsystem* ItemSubsystem = UItemBlueprintFunctionLibrary::GetItemSubsystem())
		{
			ItemSubsystem->Reset();
		}
		UGameplayStatics::OpenLevel(GetWorld(), *GetWorld()->GetMapName());
	}
}

void AShooterGameStateBase::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (UShooterGameInstance* ShooterGameInstance = Cast<UShooterGameInstance>(GetGameInstance()))
		{
			if (GetWorld())
			{
				float DiffTime = GetWorld()->GetTimeSeconds() - StartTime;

				ShooterGameInstance->TotalPlayTimeMinute = DiffTime / 60;
				ShooterGameInstance->TotalPlayTimeSec = static_cast<int32>(DiffTime) % 60;
			}
		}
		if (AWraithPlayerController* WraithPlayerController = Cast<AWraithPlayerController>(PlayerController))
		{
			WraithPlayerController->SetPause(true);
			WraithPlayerController->ShowGameOver();
		}
	}
}
