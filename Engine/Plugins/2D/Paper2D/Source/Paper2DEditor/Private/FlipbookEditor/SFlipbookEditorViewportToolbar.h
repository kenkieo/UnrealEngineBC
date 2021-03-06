// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Editor/UnrealEd/Public/SCommonEditorViewportToolbarBase.h"

// In-viewport toolbar widget used in the flipbook editor
class SFlipbookEditorViewportToolbar : public SCommonEditorViewportToolbarBase
{
public:
	SLATE_BEGIN_ARGS(SFlipbookEditorViewportToolbar) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedPtr<class ICommonEditorViewportToolbarInfoProvider> InInfoProvider);
};
