// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameExperienceManagerComponent.h"

#include "GameFeaturesSubsystemSettings.h"
#include "MyExperienceDefinition.h"
#include "MyProject/System/MyAssetManager.h"

void UMyGameExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnMyExperienceLoaded::FDelegate&& Delegate)
{
	//Experience가 로드되었는지 확인
	if (IsExperienceLoaded())
	{
		//로드가 완료 되어 있으면 그냥 실행
		Delegate.Execute(CurrentExperience);
	}
	else
	{
		//오른 값 참조 / 왼 값 참조 확인!!
		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
}

void UMyGameExperienceManagerComponent::ServerSetCurrentExperience(FPrimaryAssetId ExperienceID)
{
	UMyAssetManager& AssetManager = UMyAssetManager::Get();

	//블루프린트로 구현되어 있는 UMyExperienceDefinition 클래스를 로드한다. 
	TSubclassOf<UMyExperienceDefinition> AssetClass;
	{
		FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceID);
		//SoftObjectPath를 동기로 로드한다. 
		AssetClass = Cast<UClass>(AssetPath.TryLoad());
	}

	// UMyExperienceDefinition 클래스의 CDO를 가져온다.
	// CDO 를 가져오는 이유?
	// GetPrimaryAssetId 함수를 참고 하면, CDO 를 가져오는 이유가 설명되어 있다.
	// 간단히 말하면 CDO일 경우에만 PrimaryAssetType을 제대로 찾을 수 있다.
	const UMyExperienceDefinition* Experience = GetDefault<UMyExperienceDefinition>(AssetClass);
	check(Experience != nullptr);
	check(CurrentExperience == nullptr);
	{
		CurrentExperience = Experience;
	}
	StartExperienceLoad();
}

void UMyGameExperienceManagerComponent::StartExperienceLoad()
{
	check(CurrentExperience);
	check(LoadState == EMyExperienceLoadState::UnLoaded);

	LoadState = EMyExperienceLoadState::Loading;

	UMyAssetManager& AssetManager = UMyAssetManager::Get();

	TSet<FPrimaryAssetId> BundleAssetList;
	BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());

	TArray<FName> BundlesToLoad;
	{
		const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
		bool bLoadClient = GIsEditor || (OwnerNetMode != NM_DedicatedServer);
		bool bLoadServer = GIsEditor || (OwnerNetMode != NM_Client);

		if (bLoadClient)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
		}
		if (bLoadServer)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
		}
	}
	FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(
		this, &ThisClass::OnExperienceLoadComplete);

	TSharedPtr<FStreamableHandle> Handle = AssetManager.ChangeBundleStateForPrimaryAssets(
		BundleAssetList.Array(),
		BundlesToLoad,
		{}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority);

	if (!Handle.IsValid() || Handle->HasLoadCompleted())
	{
		FStreamableHandle::ExecuteDelegate(OnAssetsLoadedDelegate);
	}
	else
	{
		Handle->BindCancelDelegate(OnAssetsLoadedDelegate);
		Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetsLoadedDelegate]()
		{
			OnAssetsLoadedDelegate.ExecuteIfBound();
		}));
	}

	static int32 StartExperienceLoadCount = GFrameNumber;
}

void UMyGameExperienceManagerComponent::OnExperienceLoadComplete()
{
	static int32 OnExperienceLoadCompleteCount = GFrameNumber;

	OnExperienceFullLoadComplete();
}

void UMyGameExperienceManagerComponent::OnExperienceFullLoadComplete()
{
	check(LoadState != EMyExperienceLoadState::Loaded);

	LoadState = EMyExperienceLoadState::Loaded;
	OnExperienceLoaded.Broadcast(CurrentExperience);
	OnExperienceLoaded.Clear();
}

const UMyExperienceDefinition* UMyGameExperienceManagerComponent::GetCurrentExperienceChecked() const
{
	check(LoadState == EMyExperienceLoadState::Loaded);
	check(CurrentExperience != nullptr);
	return CurrentExperience;
}
