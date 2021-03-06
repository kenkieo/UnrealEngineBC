// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Paper2DEditorPrivatePCH.h"
#include "AssetData.h"

//////////////////////////////////////////////////////////////////////////
// UTileMapAssetImportData

UTileMapAssetImportData::UTileMapAssetImportData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UTileMapAssetImportData* UTileMapAssetImportData::GetImportDataForTileMap(UPaperTileMap* TileMap/*, UFbxSkeletalMeshImportData* TemplateForCreation*/)
{
	check(TileMap);

	UTileMapAssetImportData* ImportData = Cast<UTileMapAssetImportData>(TileMap->AssetImportData);
	if (ImportData == nullptr)
	{
		ImportData = ConstructObject<UTileMapAssetImportData>(UTileMapAssetImportData::StaticClass(), TileMap, NAME_None, RF_NoFlags/*, TemplateForCreation*/);

		// Try to preserve the source file path if possible
		if (TileMap->AssetImportData != nullptr)
		{
			ImportData->SourceFilePath = TileMap->AssetImportData->SourceFilePath;
			ImportData->SourceFileTimestamp = TileMap->AssetImportData->SourceFileTimestamp;
		}

		TileMap->AssetImportData = ImportData;
	}

	return ImportData;
}
