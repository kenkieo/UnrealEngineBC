// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "DebugDisplayProperty.generated.h"

/** 
 * Debug property display functionality
 * to interact with this, use "display", "displayall", "displayclear"
 *
 * @see UGameViewportClient
 * @see FDebugDisplayProperty
 * @see DrawStatsHUD
 */
USTRUCT()
struct FDebugDisplayProperty
{
	GENERATED_USTRUCT_BODY()

	/** the object whose property to display. If this is a class, all objects of that class are drawn. */
	UPROPERTY()
	class UObject* Obj;

	/** if Obj is a class and WithinClass is not NULL, further limit the display to objects that have an Outer of WithinClass */
	UPROPERTY()
	TSubclassOf<class UObject>  WithinClass;

	/** name of the property to display */
	FName PropertyName;

	/** whether PropertyName is a "special" value not directly mapping to a real property (e.g. state name) */
	uint32 bSpecialProperty:1;


	FDebugDisplayProperty()
		: Obj(NULL)
		, WithinClass(NULL)
		, bSpecialProperty(false)
	{
	}

};
