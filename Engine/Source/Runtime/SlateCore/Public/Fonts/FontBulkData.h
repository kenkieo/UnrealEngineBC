// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "FontBulkData.generated.h"

UCLASS()
class SLATECORE_API UFontBulkData : public UObject
{
	GENERATED_BODY()

public:
	/** Default constructor */
	UFontBulkData();

	/** Construct the bulk font data from the given file */
	UFontBulkData(const FString& InFontFilename);

	/** Construct the bulk font data from the given data */
	UFontBulkData(const void* const InFontData, const int32 InFontDataSizeBytes);

	/** Locks the bulk font data and returns a read-only pointer to it */
	const void* Lock(int32& OutFontDataSizeBytes) const;

	/** Unlock the bulk font data, after which point the pointer returned by Lock no longer is valid */
	void Unlock() const;

	// UObject interface
	virtual void Serialize(FArchive& Ar) override;

private:
	/** Internal bulk data */
	FByteBulkData BulkData;

	/** Critical section to prevent concurrent access when locking the internal bulk data */
	mutable FCriticalSection CriticalSection;
};
