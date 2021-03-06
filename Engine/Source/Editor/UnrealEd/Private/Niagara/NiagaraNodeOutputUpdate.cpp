// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "UnrealEd.h"
#include "BlueprintGraphDefinitions.h"
#include "GraphEditorSettings.h"

UNiagaraNodeOutputUpdate::UNiagaraNodeOutputUpdate(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UNiagaraNodeOutputUpdate::AllocateDefaultPins()
{
	const UEdGraphSchema_Niagara* Schema = GetDefault<UEdGraphSchema_Niagara>();

	UNiagaraScriptSource* Source = GetSource();

	TArray<FName> OutputNames;
	Source->GetParticleAttributes(OutputNames);

	for (int32 i = 0; i < OutputNames.Num(); i++)
	{
		UEdGraphPin* Pin = CreatePin(EGPD_Input, Schema->PC_Vector, TEXT(""), NULL, false, false, OutputNames[i].ToString());
		Pin->bDefaultValueIsIgnored = true;
	}
}

FText UNiagaraNodeOutputUpdate::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return NSLOCTEXT("NiagaraNode", "Output", "Output");
}

FLinearColor UNiagaraNodeOutputUpdate::GetNodeTitleColor() const
{
	return GetDefault<UGraphEditorSettings>()->FunctionTerminatorNodeTitleColor;
}

void UNiagaraNodeOutputUpdate::Compile(class INiagaraCompiler* Compiler, TArray<FNiagaraNodeResult>& Outputs)
{
	TArray<FName> OutputNames;
	Compiler->GetParticleAttributes(OutputNames);
	int32 NumOutputs = OutputNames.Num();

	//No output pins so we have 1 input pin per attribute.
	check(OutputNames.Num() == Pins.Num());

	TArray<TNiagaraExprPtr> InputExpressions;
	for (int32 i = 0; i < NumOutputs; ++i)
	{
		UEdGraphPin* Pin = Pins[i];
		check( Pin->Direction == EGPD_Input );

		TNiagaraExprPtr Output = Compiler->CompilePin(Pin);
		if (!Output.IsValid())
		{
			//The pin was not connected so pass through the initial value of the attribute.
			Output = Compiler->GetAttribute(OutputNames[i]);
		}

		InputExpressions.Add(Output);
	}

	check(NumOutputs == InputExpressions.Num());
	for (int32 i = 0; i < NumOutputs; ++i)
	{
		Compiler->Output(OutputNames[i], InputExpressions[i]);		
	}
}