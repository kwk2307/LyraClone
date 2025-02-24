// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyProject/Character/MyPawnData.h"
#include "MyGameModeBase.generated.h"

class UMyExperienceDefinition;
/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AMyGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	//GameState 생성이 완료 된 이후 발생하는 함수
	virtual void InitGameState() override;
	
	virtual  UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) final;

	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) final;

	void HandleMatchAssignmentIfNotExpectingOne();
	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceID);
	bool IsExperienceLoaded() const;
	void OnExperienceLoaded(const UMyExperienceDefinition* CurrentExperience);
	const UMyPawnData* GetPawnDataForController(class AController* InController) const;
};
