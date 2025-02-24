// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MyPawnData.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UMyPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UMyPawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="My|Pawn")
	TSubclassOf<APawn> PawnClass;
};
