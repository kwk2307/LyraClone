// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MyExperienceDefinition.generated.h"

/**
 * 
 */

class UMyPawnData;

UCLASS()
class MYPROJECT_API UMyExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UMyExperienceDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly,Category=Gameplay)
	TObjectPtr<UMyPawnData>	DefaultPawnData;

	UPROPERTY(EditDefaultsOnly,Category=Gameplay)
	TArray<FString> GameFeaturesToEnable;
	
};
