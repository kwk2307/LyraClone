// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Components/GameFrameworkComponentManager.h"
#include "MyProject/MyGameplayTags.h"

void UMyGameInstance::Init()
{
	Super::Init();

	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);
	if (ensure(ComponentManager))
	{
		const FMyGamePlayTags& GamePlayTags = FMyGamePlayTags::Get();

		ComponentManager->RegisterInitState(GamePlayTags.InitState_Spawned, false,FGameplayTag());
		ComponentManager->RegisterInitState(GamePlayTags.InitState_DataAvailable, false,GamePlayTags.InitState_Spawned);
		ComponentManager->RegisterInitState(GamePlayTags.InitState_DataInitialized, false,GamePlayTags.InitState_DataAvailable);
		ComponentManager->RegisterInitState(GamePlayTags.InitState_GameplayReady, false,GamePlayTags.InitState_DataInitialized);
	}
}

void UMyGameInstance::Shutdown()
{
	Super::Shutdown();
}
