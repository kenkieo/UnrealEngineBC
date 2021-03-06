// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "MacGraphicsSwitchingSettings.generated.h"

UCLASS(Config=EditorGameAgnostic)
class UMacGraphicsSwitchingSettings : public UObject
{
	GENERATED_UCLASS_BODY()
	
public:
	
	UPROPERTY(Config, EditAnywhere, Category=OpenGL)
	int32 DisplayMask;

	UPROPERTY(Config, EditAnywhere, Category=OpenGL)
	int32 RendererID;
	
	UPROPERTY(Config, EditAnywhere, Category=OpenGL)
	bool bAllowAutomaticGraphicsSwitching;
	
	UPROPERTY(Config, EditAnywhere, Category=OpenGL)
	bool bUseMultipleRenderers;
};
