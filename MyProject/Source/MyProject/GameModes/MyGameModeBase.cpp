// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"

#include "MyGameStateBase.h"
#include "MyProject/Character/MyCharacter.h"
#include "MyProject/Player/MyPlayerController.h"
#include "MyProject/Player/MyPlayerState.h"

AMyGameModeBase::AMyGameModeBase()
{
	GameStateClass = AMyGameStateBase::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
	PlayerStateClass = AMyPlayerState::StaticClass();
	DefaultPawnClass = AMyCharacter::StaticClass();
}
