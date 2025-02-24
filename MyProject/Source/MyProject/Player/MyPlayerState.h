// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyProject/Character/MyPawnData.h"
#include "MyPlayerState.generated.h"

class UMyPawnData;
class UMyExperienceDefinition;
/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void PostInitializeComponents() final;

	template<class T>
	const T* GetPawnData() const
	{
		return Cast<T>(PawnData);
	}
	void OnExprienceLoaded(const UMyExperienceDefinition* CurrentExperience);
	void SetPawnData(const UMyPawnData* InPawnData);
	
	UPROPERTY()
	TObjectPtr<const UMyPawnData> PawnData;
};
