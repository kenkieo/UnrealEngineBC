// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.


#pragma once
#include "K2Node.h"
#include "EdGraph/EdGraphNodeUtils.h" // for FNodeTextCache
#include "K2Node_VariableSetRef.generated.h"

UCLASS(MinimalAPI)
class UK2Node_VariableSetRef : public UK2Node
{
	GENERATED_UCLASS_BODY()

	// Begin UEdGraphNode interface
	virtual void AllocateDefaultPins() override;
	virtual void ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins) override;
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	// End UEdGraphNode interface

	// Begin UK2Node interface
	virtual void NotifyPinConnectionListChanged(UEdGraphPin* Pin) override;
	virtual class FNodeHandlingFunctor* CreateNodeHandler(class FKismetCompilerContext& CompilerContext) const override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FText GetMenuCategory() const override;
	virtual int32 GetNodeRefreshPriority() const override { return EBaseNodeRefreshPriority::Low_UsesDependentWildcard; }
	virtual bool IsActionFilteredOut(class FBlueprintActionFilter const& Filter) override;
	// End UK2Node interface

	/** 
	 * Changes the type of variable set by this node, based on the specified pin
	 *
	 * @param Pin				The pin to gather type information from.  If NULL, node will reset to wildcards
	 */
	void CoerceTypeFromPin(const UEdGraphPin* Pin);

	/** Returns the pin that specifies which by-ref variable to set */
	BLUEPRINTGRAPH_API UEdGraphPin* GetTargetPin() const;

	/** Returns the pin that specifies the value to set */
	BLUEPRINTGRAPH_API UEdGraphPin* GetValuePin() const;

private:
	/** Constructing FText strings can be costly, so we cache the node's title */
	FNodeTextCache CachedNodeTitle;
};

