// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Toolkits/IToolkit.h"
#include "Toolkits/AssetEditorToolkit.h"


/** Niagara Editor public interface */
class INiagaraEffectEditor : public FAssetEditorToolkit
{

public:
	virtual class UNiagaraEffect *GetEffect() const = 0;
	// ...

};


