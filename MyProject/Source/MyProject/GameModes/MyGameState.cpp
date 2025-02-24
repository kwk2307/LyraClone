// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameState.h"

#include "MyGameExperienceManagerComponent.h"

AMyGameState::AMyGameState()
{
	ExperienceManagerComponent = CreateDefaultSubobject<UMyGameExperienceManagerComponent>(
		TEXT("ExperienceManagerComponent"));
}
