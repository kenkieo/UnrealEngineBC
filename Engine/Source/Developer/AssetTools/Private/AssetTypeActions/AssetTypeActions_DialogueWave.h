// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SoundDefinitions.h"

class FAssetTypeActions_DialogueWave : public FAssetTypeActions_Base
{
public:
	// IAssetTypeActions Implementation
	virtual FText GetName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_DialogueWave", "Dialogue Wave"); }
	virtual FColor GetTypeColor() const override { return FColor(0,0,255); }
	virtual UClass* GetSupportedClass() const override { return UDialogueWave::StaticClass(); }
	virtual uint32 GetCategories() override { return EAssetTypeCategories::Sounds; }
	virtual bool CanFilter() override { return true; }
	virtual void OpenAssetEditor( const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>() ) override;
};