// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGameModeBase.h"
#include "ShooterGameStateBase.h"

AShooterGameModeBase::AShooterGameModeBase()
{
	GameStateClass = AShooterGameStateBase::StaticClass();
	/*
	DefaultPawnClass = 플레이어 클래스::StaticClass();
	PlayerControllerClass = 플레이어 컨트롤러::StaticClass();
	*/
}