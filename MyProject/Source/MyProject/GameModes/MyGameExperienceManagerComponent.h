// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "MyGameExperienceManagerComponent.generated.h"

/**
 * 
 */
enum class EMyExperienceLoadState
{
	UnLoaded,
	Loading,
	Loaded,
	Deactivating,
};

class UMyExperienceDefinition;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMyExperienceLoaded, const UMyExperienceDefinition*);

/**
 * 해당 Component는 GameState를 Owner 로 가지면서, Experience의 상태 정보를 가지고 있는 Component는이다.
 * 또한, Experience의 로딩 상태 업데이트 및 이벤트를 관리한다.
 */
UCLASS()
class MYPROJECT_API UMyGameExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	bool IsExperienceLoaded() const
	{
		return (LoadState == EMyExperienceLoadState::Loaded) && (CurrentExperience != nullptr);
	}

	void CallOrRegister_OnExperienceLoaded(FOnMyExperienceLoaded::FDelegate&& Delegate);
	
	void ServerSetCurrentExperience(FPrimaryAssetId ExperienceID);

	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnExperienceFullLoadComplete();
	const UMyExperienceDefinition* GetCurrentExperienceChecked() const;

public:
	UPROPERTY()
	TObjectPtr<const UMyExperienceDefinition> CurrentExperience;

	EMyExperienceLoadState LoadState = EMyExperienceLoadState::UnLoaded;

	FOnMyExperienceLoaded OnExperienceLoaded;
};
