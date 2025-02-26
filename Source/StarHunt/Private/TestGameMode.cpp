// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameMode.h"
#include "TestPlayer.h"
#include "Me.h"
#include "TestPlayerController.h"

ATestGameMode::ATestGameMode()
{
	DefaultPawnClass = AMe::StaticClass();
	PlayerControllerClass = ATestPlayerController::StaticClass();
}
