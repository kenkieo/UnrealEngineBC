// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SlateBasics.h"

class SRealtimeProfilerFrame;
struct FVisualizerEvent;


struct FRealtimeProfilerFPSChartFrame
{
	FRealtimeProfilerFPSChartFrame()
	:UnitFrame(0)
	,UnitRender(0)
	,UnitGame(0)
	,UnitGPU(0)
	{
	}

	FRealtimeProfilerFPSChartFrame(FRealtimeProfilerFPSChartFrame * InCopyFrom)
	:UnitFrame(InCopyFrom->UnitFrame)
	,UnitRender(InCopyFrom->UnitRender)
	,UnitGame(InCopyFrom->UnitGame)
	,UnitGPU(InCopyFrom->UnitGPU)
	{
	}

	float UnitFrame;
	float UnitRender;
	float UnitGame;
	float UnitGPU;
};

class FRealtimeProfiler
{
public:

	FRealtimeProfiler();
	~FRealtimeProfiler();
	void Update(TSharedPtr< FVisualizerEvent > InProfileData, FRealtimeProfilerFPSChartFrame * InFPSChartFrame);
	bool IsProfiling();

private:
	void MakeWindow();
	//TSharedPtr<SRealtimeProfilerFrame> Frame;
	TWeakPtr<SWindow> Window;
};

