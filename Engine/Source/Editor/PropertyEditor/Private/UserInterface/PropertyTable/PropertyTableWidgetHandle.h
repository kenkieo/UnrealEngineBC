// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "PropertyEditorPrivatePCH.h"
#include "SPropertyTable.h"

class FPropertyTableWidgetHandle: public IPropertyTableWidgetHandle{
public:
	FPropertyTableWidgetHandle(const TSharedRef<SPropertyTable>& InPropertyTableWidget)
		: PropertyTableWidget(InPropertyTableWidget) {}

	virtual ~FPropertyTableWidgetHandle() {}

	virtual void RequestRefresh() override
	{
		PropertyTableWidget->RequestRefresh();
	} 

	virtual TSharedRef<SWidget> GetWidget() override
	{
		return PropertyTableWidget;
	}

private:
	TSharedRef<SPropertyTable> PropertyTableWidget;
};