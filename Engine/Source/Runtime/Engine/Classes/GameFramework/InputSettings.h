// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "PlayerInput.h"
#include "InputSettings.generated.h"


/**
 * Project wide settings for input handling
 * 
 * @see https://docs.unrealengine.com/latest/INT/Gameplay/Input/index.html
 */
UCLASS(config=Input, defaultconfig)
class ENGINE_API UInputSettings
	: public UObject
{
	GENERATED_UCLASS_BODY()

	/** Properties of Axis controls */
	UPROPERTY(config, EditAnywhere, EditFixedSize, Category="Bindings", meta=(ToolTip="List of Axis Properties"), AdvancedDisplay)
	TArray<struct FInputAxisConfigEntry> AxisConfig;

	// Allow mouse to be used for touch
	UPROPERTY(config, EditAnywhere, Category="MouseProperties")
	uint32 bUseMouseForTouch:1;

	// Mouse smoothing control
	UPROPERTY(config, EditAnywhere, Category="MouseProperties", AdvancedDisplay)
	uint32 bEnableMouseSmoothing:1;

	// Scale the mouse based on the player camera manager's field of view
	UPROPERTY(config, EditAnywhere, Category="MouseProperties", AdvancedDisplay)
	uint32 bEnableFOVScaling:1;

	// The scaling value to multiply the field of view by
	UPROPERTY(config, EditAnywhere, Category="MouseProperties", AdvancedDisplay, meta=(editcondition="bEnableFOVScaling"))
	float FOVScale;

	/** If a key is pressed twice in this amount of time it is considered a "double click" */
	UPROPERTY(config, EditAnywhere, Category="MouseProperties", AdvancedDisplay)
	float DoubleClickTime;

	/** List of Action Mappings */
	UPROPERTY(config, EditAnywhere, Category="Bindings")
	TArray<struct FInputActionKeyMapping> ActionMappings;

	/** List of Axis Mappings */
	UPROPERTY(config, EditAnywhere, Category="Bindings")
	TArray<struct FInputAxisKeyMapping> AxisMappings;

	/** Should the touch input interface be shown always, or only when the platform has a touch screen? */
	UPROPERTY(config, EditAnywhere, Category="Mobile")
	bool bAlwaysShowTouchInterface;

	/** Whether or not to show the console on 4 finger tap, on mobile platforms */
	UPROPERTY(config, EditAnywhere, Category="Mobile")
	bool bShowConsoleOnFourFingerTap;

	/** The default on-screen touch input interface for the game (can be null to disable the onscreen interface) */
	UPROPERTY(config, EditAnywhere, Category="Mobile", meta=(AllowedClasses="TouchInterface"))
	FStringAssetReference DefaultTouchInterface;

	/** The key which opens the console. */
	UPROPERTY(config)
	FKey ConsoleKey_DEPRECATED;

	/** The keys which open the console. */
	UPROPERTY(config, EditAnywhere, Category="Console")
	TArray<FKey> ConsoleKeys;

	// UObject interface
#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif

	virtual void PostInitProperties() override;
	// End of UObject interface

	/** Programmatically add an action mapping to the project defaults */
	void AddActionMapping(const FInputActionKeyMapping& KeyMapping);

	/** Programmatically remove an action mapping to the project defaults */
	void RemoveActionMapping(const FInputActionKeyMapping& KeyMapping);

	/** Programmatically add an axis mapping to the project defaults */
	void AddAxisMapping(const FInputAxisKeyMapping& KeyMapping);

	/** Programmatically remove an axis mapping to the project defaults */
	void RemoveAxisMapping(const FInputAxisKeyMapping& KeyMapping);

	/** Flush the current mapping values to the config file */
	void SaveKeyMappings();

	/** Populate a list of all defined action names */
	void GetActionNames(TArray<FName>& ActionNames) const;

	/** Populate a list of all defined axis names */
	void GetAxisNames(TArray<FName>& AxisNames) const;

private:
	/** When changes are made to the default mappings, push those changes out to PlayerInput key maps */
	void ForceRebuildKeymaps();
};
