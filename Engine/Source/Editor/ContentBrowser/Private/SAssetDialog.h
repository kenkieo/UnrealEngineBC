// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

class SAssetDialog : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAssetDialog){}

	SLATE_END_ARGS()

	SAssetDialog();

	virtual void Construct(const FArguments& InArgs, const FSharedAssetDialogConfig& InConfig);

	virtual FReply OnKeyDown( const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent ) override;
	void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	/** Sets the delegate handler for when an open operation is committed */
	void SetOnAssetsChosenForOpen(const FOnAssetsChosenForOpen& InOnAssetsChosenForOpen);

	/** Sets the delegate handler for when a save operation is committed */
	void SetOnObjectPathChosenForSave(const FOnObjectPathChosenForSave& InOnObjectPathChosenForSave);

private:

	/** Moves keyboard focus to the name box if this is a save dialog */
	void FocusNameBox();

	/** Gets the name to display in the asset name box */
	FText GetAssetNameText() const;

	/** Fired when the asset name box text is commited */
	void OnAssetNameTextCommited(const FText& InText, ETextCommit::Type InCommitType);

	/** Gets the visibility of the name error label */
	EVisibility GetNameErrorLabelVisibility() const;

	/** Gets the text to display in the name error label */
	FString GetNameErrorLabelText() const;

	/** Handler for when a path is selected in the path view */
	void HandlePathSelected(const FString& NewPath);

	/** Handler for when a folder is entered in the asset view */
	void HandleAssetViewFolderEntered(const FString& NewPath);

	/** Determines if the confirm button (e.g. Open/Save) is enabled. */
	bool IsConfirmButtonEnabled() const;

	/** Handler for when the confirm button (e.g. Open/Save) is clicked */
	FReply OnConfirmClicked();

	/** Handler for when the cancel button is clicked */
	FReply OnCancelClicked();

	/** Handler for when an asset was selected in the asset picker */
	void OnAssetSelected(const FAssetData& AssetData);

	/* Handler for when an asset was double clicked in the asset picker */
	void OnAssetsActivated(const TArray<FAssetData>& SelectedAssets, EAssetTypeActivationMethod::Type ActivationType);

	/** Closes this dialog */
	void CloseDialog();

	void SetCurrentlySelectedPath(const FString& NewPath);

	void SetCurrentlyEnteredAssetName(const FString& NewName);

	void UpdateInputValidity();

	/** Used to commit the assets that were selected for open in this dialog */
	void ChooseAssetsForOpen(const TArray<FAssetData>& SelectedAssets);

	FString GetObjectPathForSave() const;

	/** Used to commit the object path used for saving in this dialog */
	void CommitObjectPathForSave();

private:

	/** Whether this is an open or save dialog */
	EAssetDialogType::Type DialogType;

	/** Used to update the path view after it has been created */
	FSetPathPickerPathsDelegate SetPathsDelegate;

	/** Used to update the asset view after it has been created */
	FSetARFilterDelegate SetFilterDelegate;

	/** Used to get the currently selected assets */
	FGetCurrentSelectionDelegate GetCurrentSelectionDelegate;

	/** Only assets of these classes will show up */
	TArray<FName> AssetClassNames;

	/** Fired when assets are chosen for open. Only fired in open dialogs. */
	FOnAssetsChosenForOpen OnAssetsChosenForOpen;

	/** Fired when an object path was chosen for save. Only fired in save dialogs. */
	FOnObjectPathChosenForSave OnObjectPathChosenForSave;

	/** The assets that are currently selected in the asset picker */
	TArray<FAssetData> CurrentlySelectedAssets;

	/** The name box. Only used in save dialogs. */
	TSharedPtr<SEditableTextBox> NameEditableText;

	/** The object path of the asset to save. Only used in save dialogs. */
	FString CurrentlySelectedPath;

	/** The object name of the asset to save. Only used in save dialogs. */
	FString CurrentlyEnteredAssetName;

	/** The behavior when the user chooses an existing asset. Only used in save dialogs. */
	ESaveAssetDialogExistingAssetPolicy::Type ExistingAssetPolicy;

	/** The error text from the last validity check */
	FText LastInputValidityErrorText;

	/** True if the last validity check returned that the class name/path is valid for creation */
	bool bLastInputValidityCheckSuccessful;

	/** Used to focus the name box after opening the dialog */
	bool bPendingFocusNextFrame;
};