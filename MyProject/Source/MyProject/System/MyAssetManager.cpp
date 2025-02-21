// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAssetManager.h"

#include "MyProject/MyLogChannels.h"

UMyAssetManager::UMyAssetManager()
: UAssetManager()	
{
}

void UMyAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UE_LOG(	LogMy, Display, TEXT("UMyAssetManager::StartInitialLoading()")	);
}

UMyAssetManager& UMyAssetManager::Get()
{
	check(GEngine);

	if (UMyAssetManager* Singleton = Cast<UMyAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogMy, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be of class UMyAssetManager"));

	// Fetal 로그를 출력하기 때문에 도달할 수 없음 구색갖추기 용 코드
	return *NewObject<UMyAssetManager>();
}

bool UMyAssetManager::ShouldLogAssetLoads()
{
	const TCHAR* CommandLineContent = FCommandLine::Get();
	static bool bLogAssetLogs = FParse::Param(CommandLineContent, TEXT("LogAssetLoads"));

	return bLogAssetLogs;
}

UObject* UMyAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		TUniquePtr<FScopeLogTime> LogTimeptr;
		if (ShouldLogAssetLoads())
		{
			LogTimeptr = MakeUnique<FScopeLogTime>(
				*FString::Printf(TEXT("SynchronousLoadAsset: %s"), *AssetPath.ToString()), nullptr,
				FScopeLogTime::ScopeLog_Seconds);
		}

		if (UAssetManager::IsValid())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath);
		}
		return AssetPath.TryLoad();
	}
	return nullptr;
}

void UMyAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		//AssetManager은 게임이 완전히 실행되지 않은 시점에도 사용될 수 있기 때문에 스레드 안전성을 위해 잠금을 걸어야함
		FScopeLock Lock(&SyncObject);
		LoadedAssets.Add(Asset);
	}
}
