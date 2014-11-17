// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "PaperTerrainMaterial.generated.h"

// Rule for a single section of a terrain material
USTRUCT(BlueprintType)
struct FPaperTerrainMaterialRule
{
	GENERATED_USTRUCT_BODY()

	// The sprite to use at the 'left' (closest to spline start) edge of the terrain segment
	UPROPERTY(Category=Sprite, EditAnywhere, meta=(DisplayThumbnail = "true"))
	UPaperSprite* StartCap;

	// A set of sprites to randomly choose to fill up the interior space between the caps in a terrain segment
	UPROPERTY(Category = Sprite, EditAnywhere, meta = (DisplayThumbnail = "true"))
	TArray<UPaperSprite*> Body;

	// The sprite to use at the 'right' (closest to spline end) edge of the terrain segment
	UPROPERTY(Category = Sprite, EditAnywhere, meta = (DisplayThumbnail = "true"))
	UPaperSprite* EndCap;

	// Minimum slope angle (in degrees) to apply this rule
	UPROPERTY(Category = Sprite, EditAnywhere)
	float MinimumAngle;

	// Maximum slope angle (in degrees) to apply this rule
	UPROPERTY(Category = Sprite, EditAnywhere)
	float MaximumAngle;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Category = Sprite, EditAnywhere)
	FText Description;
#endif

	FPaperTerrainMaterialRule()
		: StartCap(nullptr)
		, EndCap(nullptr)
		, MinimumAngle(0.0f)
		, MaximumAngle(360.0f)
	{
	}
};

/**
 * Paper Terrain Material
 *
 * 'Material' setup for a 2D terrain spline (stores references to sprites that will be instanced along the spline path, not actually related to UMaterialInterface).
 */

UCLASS(BlueprintType)
class PAPER2D_API UPaperTerrainMaterial : public UDataAsset
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(Category=Sprite, EditAnywhere, meta=(DisplayThumbnail = "true"))
	TArray<FPaperTerrainMaterialRule> Rules;

	// The sprite to use for an interior region fill
	UPROPERTY(Category = Sprite, EditAnywhere, meta = (DisplayThumbnail = "true"))
	UPaperSprite* InteriorFill;

#if WITH_EDITOR
	// UObject interface
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	// End of UObject interface
#endif
};