// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "K2Node_CallFunction.h"
#include "K2Node_CallDataTableFunction.generated.h"

UCLASS(MinimalAPI)
class UK2Node_CallDataTableFunction : public UK2Node_CallFunction
{
	GENERATED_UCLASS_BODY()

	// Begin EdGraphNode interface
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	virtual void NotifyPinConnectionListChanged(UEdGraphPin* Pin) override;
	// End EdGraphNode interface
};