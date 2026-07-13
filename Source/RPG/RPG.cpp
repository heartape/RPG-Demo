// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPG.h"
#include "Modules/ModuleManager.h"
#include "AI/RPGGameplayTags.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FRPGModule, RPG, "RPG" );

void FRPGModule::StartupModule()
{
	FDefaultGameModuleImpl::StartupModule();
	FRPGGameplayTags::InitializeNativeTags();
}
