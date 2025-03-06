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

		if (UShooterGameInstance* ShooterGameInstance = Cast<UShooterGameInstance>(GetGameInstance()))
		{
			ShooterGameInstance->CurrentWave++;
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
		if (UShooterGameInstance* ShooterGameInstance = Cast<UShooterGameInstance>(GetGameInstance()))
		{
			ShooterGameInstance->CurrentWave = 0;
			ShooterGameInstance->TotalPlayTimeMinute = 0;
			ShooterGameInstance->TotalPlayTimeSec = 0;
			ShooterGameInstance->TotalScore = 0;
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

				ShooterGameInstance->TotalPlayTimeSec += static_cast<int32>(DiffTime) % 60;
				if (ShooterGameInstance->TotalPlayTimeSec >= 60)
				{
					ShooterGameInstance->TotalPlayTimeSec -= 60;
					ShooterGameInstance->TotalPlayTimeMinute++;
				}
				ShooterGameInstance->TotalPlayTimeMinute += DiffTime / 60;
			}
		}
		if (AWraithPlayerController* WraithPlayerController = Cast<AWraithPlayerController>(PlayerController))
		{
			WraithPlayerController->SetPause(true);
			WraithPlayerController->ShowGameOver();
		}
	}
}

void AShooterGameStateBase::OnGameClear()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (UShooterGameInstance* ShooterGameInstance = Cast<UShooterGameInstance>(GetGameInstance()))
		{
			if (GetWorld())
			{
				float DiffTime = GetWorld()->GetTimeSeconds() - StartTime;

				ShooterGameInstance->TotalPlayTimeSec += static_cast<int32>(DiffTime) % 60;
				if (ShooterGameInstance->TotalPlayTimeSec >= 60)
				{
					ShooterGameInstance->TotalPlayTimeSec -= 60;
					ShooterGameInstance->TotalPlayTimeMinute++;
				}
				ShooterGameInstance->TotalPlayTimeMinute += DiffTime / 60;
			}
		}
		if (AWraithPlayerController* WraithPlayerController = Cast<AWraithPlayerController>(PlayerController))
		{
			WraithPlayerController->SetPause(true);
			WraithPlayerController->ShowGameClear();
		}
	}
}
