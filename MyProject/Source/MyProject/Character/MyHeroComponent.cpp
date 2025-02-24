// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHeroComponent.h"

#include "MyPawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "MyProject/MyGameplayTags.h"
#include "MyProject/MyLogChannels.h"
#include "MyProject/Player/MyPlayerState.h"

UMyHeroComponent::UMyHeroComponent(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UMyHeroComponent::OnRegister()
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

void UMyHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	BindOnActorInitStateChanged(UMyPawnExtensionComponent::NAME_ActorFeatureName,FGameplayTag(),false);

	ensure(TryToChangeInitState(FMyGamePlayTags::Get().InitState_Spawned));

	CheckDefaultInitialization();
}

void UMyHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	
	Super::EndPlay(EndPlayReason);
}

void UMyHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	const FMyGamePlayTags& InitTags = FMyGamePlayTags::Get();

	if (Params.FeatureName == UMyPawnExtensionComponent::NAME_ActorFeatureName)
	{
		// HakPawnExtensionComponent�� DataInitialized ���� ��ȭ ���� ��, HakHeroComponent�� DataInitialized ���·� ����
		// - CanChangeInitState Ȯ��
		if (Params.FeatureState == InitTags.InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

bool UMyHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	check(Manager);

	const FMyGamePlayTags& InitTags = FMyGamePlayTags::Get();
	APawn* Pawn = GetPawn<APawn>();
	AMyPlayerState* HakPS = GetPlayerState<AMyPlayerState>();

	// Spawned �ʱ�ȭ
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
		if (!HakPS)
		{
			return false;
		}

		return true;
	}

	// DataAvailable -> DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// PawnExtensionComponent�� DataInitialized�� ������ ��ٸ� (== ��� Feature Component�� DataAvailable�� ����)
		return HakPS && Manager->HasFeatureReachedInitState(Pawn, UMyPawnExtensionComponent::NAME_ActorFeatureName, InitTags.InitState_DataInitialized);
	}

	// DataInitialized -> GameplayReady
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void UMyHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState)
{
	const FMyGamePlayTags& InitTags = FMyGamePlayTags::Get();

	// DataAvailable -> DataInitialized �ܰ�
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		AMyPlayerState* HakPS = GetPlayerState<AMyPlayerState>();
		if (!ensure(Pawn && HakPS))
		{
			return;
		}
	}
}

void UMyHeroComponent::CheckDefaultInitialization()
{
	const FMyGamePlayTags& InitTags = FMyGamePlayTags::Get();

	static const TArray<FGameplayTag> StateChain = {
		InitTags.InitState_Spawned,
		InitTags.InitState_DataAvailable,
		InitTags.InitState_DataInitialized,
		InitTags.InitState_GameplayReady
	};
	ContinueInitStateChain(StateChain);
}
