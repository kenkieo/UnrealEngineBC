// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "EnginePrivate.h"
#include "GameFramework/SimpleReticle.h"

void FSimpleReticle::Draw( UCanvas* InCanvas, FLinearColor InColor )
{
	FVector2D CanvasCenter( InCanvas->OrgX + ((InCanvas->ClipX - InCanvas->OrgX) / 2.0f), InCanvas->OrgX + ((InCanvas->ClipY - InCanvas->OrgY) / 2.0f) );
	FCanvasLineItem LineItem( CanvasCenter, FVector2D(0.0f, 0.0f) );
	LineItem.SetColor( InColor );
	LineItem.Draw( InCanvas->Canvas, CanvasCenter - HorizontalOffsetMin, CanvasCenter - HorizontalOffsetMax );
	LineItem.Draw( InCanvas->Canvas, CanvasCenter + HorizontalOffsetMin, CanvasCenter + HorizontalOffsetMax );
	LineItem.Draw( InCanvas->Canvas, CanvasCenter - VerticalOffsetMin, CanvasCenter - VerticalOffsetMax );
	LineItem.Draw( InCanvas->Canvas, CanvasCenter + VerticalOffsetMin, CanvasCenter + VerticalOffsetMax );
}