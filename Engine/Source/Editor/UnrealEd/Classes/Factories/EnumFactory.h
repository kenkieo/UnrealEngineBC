// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EnumFactory.generated.h"

UCLASS(hidecategories=Object, MinimalAPI)
class UEnumFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

	// Begin UFactory Interface
	virtual UObject* FactoryCreateNew(UClass* Class,UObject* InParent,FName Name,EObjectFlags Flags,UObject* Context,FFeedbackContext* Warn) override;
	// Begin UFactory Interface
};



