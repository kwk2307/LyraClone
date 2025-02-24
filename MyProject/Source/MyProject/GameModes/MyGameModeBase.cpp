// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"

#include "MyExperienceDefinition.h"
#include "MyGameExperienceManagerComponent.h"
#include "MyGameState.h"
#include "MyProject/Character/MyCharacter.h"
#include "MyProject/Player/MyPlayerController.h"
#include "MyProject/Player/MyPlayerState.h"

AMyGameModeBase::AMyGameModeBase()
{
	GameStateClass = AMyGameState::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
	PlayerStateClass = AMyPlayerState::StaticClass();
	DefaultPawnClass = AMyCharacter::StaticClass();
}

void AMyGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	//InitGame시점에서는 준비되어있는 게임프레임워크 요소들이 존재하지 않기 때문
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AMyGameModeBase::HandleMatchAssignmentIfNotExpectingOne);
}

void AMyGameModeBase::InitGameState()
{
	Super::InitGameState();

	UMyGameExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<
		UMyGameExperienceManagerComponent>();

	check(ExperienceManagerComponent);

	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(
		FOnMyExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

UClass* AMyGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (const UMyPawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void AMyGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

APawn* AMyGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer,
                                                                   const FTransform& SpawnTransform)
{
	return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
}

void AMyGameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{
	FPrimaryAssetId ExperienceID;

	UWorld* World = GetWorld();

	if (!ExperienceID.IsValid())
	{
		ExperienceID = FPrimaryAssetId(FPrimaryAssetType("MyExperienceDefinition"), TEXT("B_MyDefaultExperience"));
	}
	OnMatchAssignmentGiven(ExperienceID);
}

void AMyGameModeBase::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceID)
{
	check(ExperienceID.IsValid());

	UMyGameExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<
		UMyGameExperienceManagerComponent>();
	check(ExperienceManagerComponent);
	ExperienceManagerComponent->ServerSetCurrentExperience(ExperienceID);
}

bool AMyGameModeBase::IsExperienceLoaded() const
{
	check(GameState);
	UMyGameExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<
		UMyGameExperienceManagerComponent>();

	return ExperienceManagerComponent->IsExperienceLoaded();
}

void AMyGameModeBase::OnExperienceLoaded(const UMyExperienceDefinition* CurrentExperience)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		AMyPlayerController* PC = Cast<AMyPlayerController>(Iterator->Get());
		if (PC && PC->GetPawn() == nullptr)
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}

const UMyPawnData* AMyGameModeBase::GetPawnDataForController(class AController* InController) const
{
	if (InController)
	{
		if (const AMyPlayerState* HasPS = InController->GetPlayerState<AMyPlayerState>())
		{
			if (const UMyPawnData* PawnData = HasPS->GetPawnData<UMyPawnData>())
			{
				return PawnData;
			}
		}
	}
	check(GameState);
	UMyGameExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<
		UMyGameExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	if (ExperienceManagerComponent->IsExperienceLoaded())
	{
		const UMyExperienceDefinition* Experience = ExperienceManagerComponent->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData)
		{
			return Experience->DefaultPawnData;
		}
	}
	return nullptr;
}
