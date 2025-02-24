// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/UnrealString.h"
#include "Containers/Map.h"

#include "GameplayTagContainer.h"

class UGameplayTagsManager;

struct FMyGamePlayTags
{
	static const FMyGamePlayTags& Get() { return GamePlayTags; }
	static void InitializeNativeTags();

	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
	void AddAllTags(UGameplayTagsManager& Manager);

	FGameplayTag InitState_Spawned;
	FGameplayTag InitState_DataAvailable;
	FGameplayTag InitState_DataInitialized;
	FGameplayTag InitState_GameplayReady;

private:
	static FMyGamePlayTags GamePlayTags;
};
