// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawnExtensionComponent.h"

#include "Components/GameFrameworkComponentManager.h"
#include "MyProject/MyGameplayTags.h"
#include "MyProject/MyLogChannels.h"

const FName UMyPawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

UMyPawnExtensionComponent::UMyPawnExtensionComponent(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UMyPawnExtensionComponent::OnRegister()
{
	// OnRegister에서 Actor에 컴포넌트를 부착함
	Super::OnRegister();
	{
		if (!GetPawn<APawn>())
		{
			UE_LOG(LogMy, Error, TEXT("UMyPawnExtensionComponent::OnRegister: Pawn is not valid"));
			return;
		}
	}

	RegisterInitStateFeature();
}

void UMyPawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();

	//모든 등록된 Feature Component가 InitState를 변경할 때 OnActorInitStateChanged()를 호출하도록 함
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(),false);

	ensure(TryToChangeInitState(FMyGamePlayTags::Get().InitState_Spawned));

	CheckDefaultInitialization();
}

void UMyPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	
	Super::EndPlay(EndPlayReason);
}

void UMyPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		const FMyGamePlayTags& InitTags = FMyGamePlayTags::Get();
		if (Params.FeatureState == InitTags.InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

bool UMyPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();
	const FMyGamePlayTags& InitTags = FMyGamePlayTags::Get();
	
	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		if (Pawn)
		{
			return true;
		}
	}

	// Spawned -> DataAvailable
	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		if (!PawnData)
		{
			return false;
		}
		
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		if (bIsLocallyControlled)
		{
			if (!GetController<AController>())
			{
				return false;
			}
		}

		return true;
	}

	// DataAvailable -> DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		return Manager->HaveAllFeaturesReachedInitState(Pawn, InitTags.InitState_DataAvailable);
	}

	// DataInitialized -> GameplayReady
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}
	
	return false;
}

void UMyPawnExtensionComponent::CheckDefaultInitialization()
{
	//모든 등록된 
	CheckDefaultInitializationForImplementers();
	
	const FMyGamePlayTags& InitTags = FMyGamePlayTags::Get();

	static const TArray<FGameplayTag> StateChain = {
		InitTags.InitState_Spawned,
		InitTags.InitState_DataAvailable,
		InitTags.InitState_DataInitialized,
		InitTags.InitState_GameplayReady
	};
	ContinueInitStateChain(StateChain);
}
