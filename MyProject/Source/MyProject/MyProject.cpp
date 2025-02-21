// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProject.h"

#include "MyLogChannels.h"
#include "Modules/ModuleManager.h"

class FMyProjectModule : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

void FMyProjectModule::StartupModule()
{
	
	FDefaultGameModuleImpl::StartupModule();
	// Put your startup code here
	UE_LOG(LogMy, Display, TEXT("Hello World!"));
}

void FMyProjectModule::ShutdownModule()
{
	FDefaultGameModuleImpl::ShutdownModule();
	// Put your shutdown code here
}

//반드시 IMPLEMENT_PRIMARY_GAME_MODULE의 경우 하나만 존재해야함
IMPLEMENT_PRIMARY_GAME_MODULE( FMyProjectModule, MyProject, "MyProject" );
