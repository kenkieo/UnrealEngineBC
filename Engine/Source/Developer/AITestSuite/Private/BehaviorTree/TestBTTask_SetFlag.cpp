// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "AITestSuitePrivatePCH.h"
#include "BehaviorTree/TestBTTask_SetFlag.h"
#include "BehaviorTree/BlackboardComponent.h"

UTestBTTask_SetFlag::UTestBTTask_SetFlag(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Log";
	TaskResult = EBTNodeResult::Succeeded;
	KeyName = TEXT("Bool1");
	bValue = true;
}

EBTNodeResult::Type UTestBTTask_SetFlag::ExecuteTask(UBehaviorTreeComponent* OwnerComp, uint8* NodeMemory)
{
	OwnerComp->GetBlackboardComponent()->SetValueAsBool(KeyName, bValue);
	return TaskResult;
}
