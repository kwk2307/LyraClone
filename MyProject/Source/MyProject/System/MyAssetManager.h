// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "MyAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UMyAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UMyAssetManager();

	virtual void StartInitialLoading() override;

	static UMyAssetManager& Get();

	static bool ShouldLogAssetLoads();

	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);

	template <typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	template <typename AssetType>
	static TSubclassOf<AssetType> GetSubClass(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	void AddLoadedAsset(const UObject* Asset);

	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;

	FCriticalSection SyncObject;
};

template <typename AssetType>
AssetType* UMyAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	AssetType* LoadedAsset = nullptr;
	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();
	if (AssetPath.IsValid())
	{
		LoadedAsset = AssetPointer.Get();
		if (!LoadedAsset)
		{
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			//에셋이 정상적으로 로드되었는지 확인하고, 로드에 실패하면 해당 에셋 경로를 포함한 오류 메시지를 기록함
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset %s"), *AssetPath.ToString());;
		}
		if (LoadedAsset && bKeepInMemory)
		{
			// 메모리에 상주하게 하기 위한 장치
			// 한번 등록되면 직접 내리지 않는 한 unload되지 않음
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	}
	return LoadedAsset;
}

template <typename AssetType>
TSubclassOf<AssetType> UMyAssetManager::GetSubClass(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubClass = nullptr;
	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();
	if (AssetPath.IsValid())
	{
		LoadedSubClass = AssetPointer.Get();
		if (!LoadedSubClass)
		{
			LoadedSubClass = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			//에셋이 정상적으로 로드되었는지 확인하고, 로드에 실패하면 해당 에셋 경로를 포함한 오류 메시지를 기록함
			ensureAlwaysMsgf(LoadedSubClass, TEXT("Failed to load asset %s"), *AssetPath.ToString());;
		}
		if (LoadedSubClass && bKeepInMemory)
		{
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubClass));
		}
	}
	return LoadedSubClass;
}
