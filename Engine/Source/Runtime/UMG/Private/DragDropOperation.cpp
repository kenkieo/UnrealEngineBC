// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "UMGPrivatePCH.h"

/////////////////////////////////////////////////////
// UDragDropOperation

UDragDropOperation::UDragDropOperation(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDragDropOperation::Drop_Implementation(const FPointerEvent& PointerEvent)
{
	OnDrop.Broadcast(this);
}

void UDragDropOperation::DragCancelled_Implementation(const FPointerEvent& PointerEvent)
{
	OnDragCancelled.Broadcast(this);
}

void UDragDropOperation::Dragged_Implementation(const FPointerEvent& PointerEvent)
{
	OnDragged.Broadcast(this);
}
