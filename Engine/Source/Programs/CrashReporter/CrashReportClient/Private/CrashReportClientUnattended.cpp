// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "CrashReportClientApp.h"
#include "CrashReportClientUnattended.h"

FCrashReportClientUnattended::FCrashReportClientUnattended(const FPlatformErrorReport& ErrorReport)
	: Uploader(GServerIP)
{
	Uploader.BeginUpload(ErrorReport);

	// Prevent uploader waiting for a diagnosis file
	Uploader.LocalDiagnosisSkipped();

	StartTicker();
}

bool FCrashReportClientUnattended::Tick(float UnusedDeltaTime)
{
	if (Uploader.IsFinished())
	{
		FPlatformMisc::RequestExit(false /* don't force */);
		// No more ticks, thank you
		return false;
	}

	return true;	
}

void FCrashReportClientUnattended::StartTicker()
{
	FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateRaw(this, &FCrashReportClientUnattended::Tick), 1.f);
}
