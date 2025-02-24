// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"

#include "MyProject/GameModes/MyGameExperienceManagerComponent.h"
#include "MyProject/GameModes/MyGameModeBase.h"

void AMyPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AGameStateBase* Gamestate = GetWorld()->GetGameState();
	check(Gamestate);

	// PlayerState 는 GameState 이후에 생성이 됨 -> Unreal Engine GameplayFrameWork 을 확인했을 때
	// 그렇기 때문에 GameState에서 ExperienceManagerComponent 를 찾아서 가져올 수 있음
	UMyGameExperienceManagerComponent* ExperienceManagerComponent = Gamestate->FindComponentByClass<
		UMyGameExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(
		FOnMyExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExprienceLoaded));
}

void AMyPlayerState::OnExprienceLoaded(const UMyExperienceDefinition* CurrentExperience)
{
	if (AMyGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AMyGameModeBase>())
	{
		const UMyPawnData* NewPawnData = GameMode->GetPawnDataForController(GetOwningController());
		check(NewPawnData);

		SetPawnData(NewPawnData);
	}
}

void AMyPlayerState::SetPawnData(const UMyPawnData* InPawnData)
{
	check(InPawnData);

	check(!PawnData);

	PawnData = InPawnData;
}
