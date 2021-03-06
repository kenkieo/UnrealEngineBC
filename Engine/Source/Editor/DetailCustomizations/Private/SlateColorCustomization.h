// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MathStructCustomizations.h"

class FSlateColorCustomization : public FColorStructCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	/** IPropertyTypeCustomization interface */
	virtual void CustomizeHeader( TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils ) override;

	virtual void MakeHeaderRow(TSharedRef<class IPropertyHandle>& InStructPropertyHandle, FDetailWidgetRow& Row) override;

private:

	/**
	 * Called when the value is changed in the property editor
	 */
	virtual void OnValueChanged();

	ESlateCheckBoxState::Type GetForegroundCheckState() const;

	void HandleForegroundChanged(ESlateCheckBoxState::Type CheckedState);

private:

	/** slate color struct handle */
	TSharedPtr<IPropertyHandle> StructPropertyHandle;

	TSharedPtr<IPropertyHandle> ColorRuleHandle;
	TSharedPtr<IPropertyHandle> SpecifiedColorHandle;
};

