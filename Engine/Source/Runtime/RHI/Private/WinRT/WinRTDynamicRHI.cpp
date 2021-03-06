// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "RHI.h"
#include "ModuleManager.h"

#if USE_DYNAMIC_RHI

FDynamicRHI* PlatformCreateDynamicRHI()
{
	FDynamicRHI* DynamicRHI = NULL;

	// Load the dynamic RHI module.
	IDynamicRHIModule* DynamicRHIModule = &FModuleManager::LoadModuleChecked<IDynamicRHIModule>(TEXT("D3D11RHI"));

	// Create the dynamic RHI.
	if (!DynamicRHIModule->IsSupported())
	{
		FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("WinRT", "FailedToCreateWinRT_RHI", "WinRTRHI failure?"));
		FPlatformMisc::RequestExit(1);
	}
	else
	{
		DynamicRHI = DynamicRHIModule->CreateRHI();
	}

	return DynamicRHI;
}

#endif // USE_DYNAMIC_RHI
