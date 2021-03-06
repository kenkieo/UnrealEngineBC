// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "GameFramework/PhysicsVolume.h"
#include "KillZVolume.generated.h"

UCLASS()
class ENGINE_API AKillZVolume : public APhysicsVolume
{
	GENERATED_UCLASS_BODY()
	
	//Begin PhysicsVolume Interface
	virtual void ActorEnteredVolume(class AActor* Other) override;
	//End PhysicsVolume Interface
};



