// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "AIModulePrivate.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "EnvironmentQuery/Generators/EnvQueryGenerator_BlueprintBase.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "AI/Navigation/RecastNavMesh.h"
#include "VisualLogger/VisualLogger.h"

#define LOCTEXT_NAMESPACE "EnvQueryGenerator"

UEnvQueryGenerator_BlueprintBase::UEnvQueryGenerator_BlueprintBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Context = UEnvQueryContext_Querier::StaticClass();
	ItemType = UEnvQueryItemType_Actor::StaticClass();
	Radius.Value = 1000.0f;
	GeneratedItemType = UEnvQueryItemType_Actor::StaticClass();
}

void UEnvQueryGenerator_BlueprintBase::PostInitProperties()
{
	Super::PostInitProperties();
	ItemType = GeneratedItemType;
}

UWorld* UEnvQueryGenerator_BlueprintBase::GetWorld() const
{
	return CachedQueryInstance ? CachedQueryInstance->World : NULL;
}

void UEnvQueryGenerator_BlueprintBase::GenerateItems(FEnvQueryInstance& QueryInstance) const
{
	float RadiusValue = 0.f;
	if (QueryInstance.GetParamValue(Radius, RadiusValue, TEXT("Radius")) == false)
	{
		return;
	}

	TArray<FVector> ContextLocations;
	QueryInstance.PrepareContext(Context, ContextLocations);

	CachedQueryInstance = &QueryInstance;
	DoItemGeneration(ContextLocations);
	CachedQueryInstance = NULL;
}

void UEnvQueryGenerator_BlueprintBase::AddGeneratedVector(FVector Vector)
{
	check(CachedQueryInstance);
	ensure(ItemType->GetClass()->IsChildOf(UEnvQueryItemType_VectorBase::StaticClass()));
	CachedQueryInstance->AddItemData<UEnvQueryItemType_Point>(Vector);
}

void UEnvQueryGenerator_BlueprintBase::AddGeneratedActor(AActor* Actor)
{
	check(CachedQueryInstance);
	//ensure(ItemType->GetClass()->IsChildOf(UEnvQueryItemType_ActorBase::StaticClass()));
	CachedQueryInstance->AddItemData<UEnvQueryItemType_Actor>(Actor);
}

UObject* UEnvQueryGenerator_BlueprintBase::GetQuerier() const
{
	check(CachedQueryInstance);
	return CachedQueryInstance->Owner.Get();
}

FText UEnvQueryGenerator_BlueprintBase::GetDescriptionTitle() const
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("DescriptionTitle"), Super::GetDescriptionTitle());
	Args.Add(TEXT("DescribeGeneratorAction"), GeneratorsActionDescription);
	Args.Add(TEXT("DescribeContext"), UEnvQueryTypes::DescribeContext(Context));

	return FText::Format(LOCTEXT("DescriptionBlueprintImplementedGenerator", "{DescriptionTitle}: {DescribeGeneratorAction} around {DescribeContext}"), Args);
}

FText UEnvQueryGenerator_BlueprintBase::GetDescriptionDetails() const
{
	return FText::FromString(TEXT("None"));
}

#undef LOCTEXT_NAMESPACE