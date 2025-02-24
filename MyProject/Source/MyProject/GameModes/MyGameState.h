// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameState.generated.h"


class UMyGameExperienceManagerComponent;
/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	AMyGameState();

	UPROPERTY()
	TObjectPtr<UMyGameExperienceManagerComponent> ExperienceManagerComponent;
};
